#include <3ds.h>
#include <stdio.h>
#include <sys/dirent.h>
#include <string>
#include <sstream>
#include <fstream>
#include <duktape.h>
#include <dukglue/dukglue.h>
#include "fs.h"
#include "pp2d/pp2d.h"
#include "jsLibs/Sound.h"
#include "jsLibs/gfxlib.h"

//Uncomment this to run code from romfs
//when running from romfs the app will close at the end of the last script
//#define RUN_FROM_ROMFS 

#ifndef RUN_FROM_ROMFS
#include "bg.h"
#endif

std::string JsDefines;
std::string RunCode;
std::string ScriptPath;
duk_context *mainCtx = NULL;

void EndlessLoop()
{
	while (true)
	{	
		svcSleepThread(1000000);
	}
}

inline void WaitKey()
{
	while (aptMainLoop())
	{
		hidScanInput();
		if (hidKeysDown() != 0) break;
		svcSleepThread(5000);
	}
}

//From duktape docs:
void duk_fatal_error(void *udata, const char *msg) {
    (void) udata;  
    printf("[DUK FATAL]: %s\n", (msg ? msg : "no message"));
	EndlessLoop();
}

static duk_ret_t get_stack_raw(duk_context *ctx, void *udata) {
	(void)udata;

	if (!duk_is_object(ctx, -1)) {
		return 1;
	}
	if (!duk_has_prop_string(ctx, -1, "stack")) {
		return 1;
	}
	if (!duk_is_error(ctx, -1)) {
		/* Not an Error instance, don't read "stack". */
		return 1;
	}

	duk_get_prop_string(ctx, -1, "stack");  /* caller coerces */
	duk_remove(ctx, -2);
	return 1;
}

static void print_pop_error(duk_context *ctx) {
	(void)duk_safe_call(ctx, get_stack_raw, NULL, 1, 1 );
	printf( "%s\n", duk_safe_to_string(ctx, -1));
	duk_pop(ctx);
}

extern void SetBindings(duk_context* ctx); //from bindings.cpp
extern void BottomScreenEnableConsole();
extern void DisposeResources();

void InitJsCtx()
{
	if (mainCtx) 
	{
		duk_destroy_heap(mainCtx);
		printf("destroyed context\n");
	}
	mainCtx = duk_create_heap(NULL, NULL, NULL, NULL, duk_fatal_error);
	if (mainCtx) printf("context created\n");	
	SetBindings(mainCtx);		
	printf("set bindings\n");
	duk_peval_string(mainCtx, JsDefines.c_str());
	printf("Js defs loaded\n");
}

bool ClearContex = false;
void RunFile(const char * path)
{
	#ifndef RUN_FROM_ROMFS
		ClearBgWave();
	#endif

	printf("run js %s\n", path);
	loadCode(path, &RunCode);
	ScriptPath.assign (path);
	int res = duk_peval_string(mainCtx, RunCode.c_str());
	DisposeResources();
	if (res != 0){
    	printf("Script error:\n");
		print_pop_error(mainCtx);
	}
	printf("[script finished]\n");
	if (!ClearContex) InitJsCtx();

	#ifndef RUN_FROM_ROMFS
		InitBgWave(1);
	#endif
}

void RunFileInternal(const char * path)
{
	loadCode(path, &RunCode);
	//ScriptPath.assign (path);
	if (duk_peval_string(mainCtx, RunCode.c_str())!= 0)
	{
    	printf("Script error:\n");
		print_pop_error(mainCtx);
	}
}

int main()
{
	osSetSpeedupEnable(true);
	romfsInit();
	sdmcInit();
	pp2d_init();
	SoundInit();
	consoleInit(GFX_BOTTOM, NULL);	
	printf("started\n");
	loadCode("romfs:/definitions.js", &JsDefines);
	InitJsCtx();
	
	printf("\n\n\n-------------------------\nNo Name Js Interpreter - by exelix11\n");
	printf("Powered by:\nhttp://duktape.org/\nhttps://github.com/Aloshi/dukglue\npp2d by BernardoGiordano\n");	
	printf("Ready !\n");
#ifdef RUN_FROM_ROMFS
	RunFile("romfs:/gputest.js");
#else
	InitBgWave(1);

	std::vector<FileInfo> filesOnSd = fsGetDirectoryContents("sdmc:/js");

	u16 fileIndex = 0;
	int ScreenSpace = (SCREEN_HEIGHT - 64)/16 - 3;
	u8 keyDown = 0;
	while(aptMainLoop())
	{
		hidScanInput();
		keyDown = hidKeysDown();
		
		pp2d_begin_draw(GFX_TOP,GFX_LEFT);
		DrawBg();
		if (filesOnSd.size() != 0 )
		{
			pp2d_draw_text(0,0,.5f,.5f,0xFFFFFFFF, "A: launch file \nSTART: quit\n\n");
			pp2d_draw_text(0,48,.5f,.5f,0xFFFFFFFF,"File list:\n");

			int ScreenBaseIndex = fileIndex < ScreenSpace ? 0 : fileIndex - ScreenSpace;
			for (int i = ScreenBaseIndex; i < filesOnSd.size(); i++)
			{
                if (i - ScreenBaseIndex > ScreenSpace) break;
				pp2d_draw_textf(0,64 + 16* (i -ScreenBaseIndex) ,.5f,.5f,0xFFFFFFFF,
								"%s %s\n", (i == fileIndex ? ">" : " ") ,
								filesOnSd[i].name.c_str());
			}

			if (keyDown & KEY_UP) {if(fileIndex == 0) fileIndex = filesOnSd.size() - 1; else --fileIndex;}
			else if (keyDown & KEY_DOWN) {if(fileIndex == filesOnSd.size() -1) fileIndex = 0; else ++fileIndex;}
			else if (keyDown & KEY_A) 
			{
				pp2d_end_draw();
				gspWaitForVBlank();
				RunFile(filesOnSd[fileIndex].path.c_str());
				continue;
			}
			else if (keyDown & KEY_SELECT)
			{
				ClearContex = !ClearContex;
				InitJsCtx();
			}
		}
		else 
		{
			pp2d_draw_text(0,0,1,1,0xFFFFFFFF, "sd:/js not found or empty");
		}
			
		if (keyDown & KEY_START) break;

		pp2d_end_draw();
		gspWaitForVBlank();
	}
#endif
	
	duk_destroy_heap(mainCtx);
	printf("destroyed context\n");	

	romfsExit();
	sdmcExit();
	pp2d_exit();
	SoundDispose();
	return 0;
}