#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <dukglue/dukglue.h>
#include "jsLibs/gfxlib.h"
#include "pp2d/loadbmp.h"
#include "pp2d/lodepng.h"
#include "pp2d/pp2d.h"
#include "jsLibs/FastLibDefine.h"
#include "jsLibs/jsGPU.h"
#include "jsLibs/Sound.h"
#include "jsLibs/touch.h"
#include "jsLibs/jsFS.h"

//Custom functions
void MyGspWaitForVBlank(){gspWaitForEvent(GSPGPU_EVENT_VBlank0, true);} //In ctrulib it's defined as a macro
void print(const char* s) {printf("%s\n",s);}
void ConsoleCls() {printf("\x1b[2J");}

void BottomScreenEnableConsole()
{	
	consoleInit(GFX_BOTTOM, NULL);	
}

void BottomScreenDisableConsole()
{	
	gfxSetScreenFormat(GFX_BOTTOM, GSP_BGR8_OES);
	gfxSetDoubleBuffering(GFX_BOTTOM,true);
	gfxSwapBuffersGpu();
	gspWaitForVBlank();
}

extern std::string ScriptPath; //from main.cpp
std::string ScriptResFolder()
{
	std::stringstream ss;
	ss << ScriptPath << "-res";
	return ss.str();
}

std::string GetScriptPath()
{
	size_t found;
	found=ScriptPath.find_last_of("/\\");
	return ScriptPath.substr(0,found);
}

extern void RunFileInternal(const char * path);

void SetSoundBindings(duk_context *ctx)
{
	dukglue_register_function(ctx, &SoundStopChannel, "SoundStopChannel");
	dukglue_register_function(ctx, &SoundStopAllChannels, "SoundStopAllChannels");
	dukglue_register_function(ctx, &SoundFreeWave, "SoundFreeWave");
	dukglue_register_function(ctx, &SoundFreeAll, "SoundFreeAll");

	dukglue_register_function(ctx, &SoundLoadWave, "SoundLoadWave");
	dukglue_register_method(ctx, &WavFile::PlayOnChannel, "PlayOnChannel");
	dukglue_register_method(ctx, &WavFile::Play, "Play");
}

void SetGfxLibBindings(duk_context *ctx)
{
	dukglue_register_function(ctx, &GETLIBFN(GfxLib), "GetGfxLib");
	dukglue_register_method(ctx, &GfxLib::setPixel, "setPixel");
	dukglue_register_method(ctx, &GfxLib::setPixelRGB, "setPixelRGB");
	dukglue_register_method(ctx, &GfxLib::EndFrame, "EndFrame");
	
	dukglue_register_method(ctx, &GfxLib::ClearFb, "ClearFb");
}

void SetPP2DBindings(duk_context *ctx)
{		
	dukglue_register_function(ctx, &GETLIBFN(jsGPU), "GetJsGpu");
	dukglue_register_method(ctx, &jsGPU::begin_draw, "begin_draw");
	dukglue_register_method(ctx, &jsGPU::draw_on, "draw_on");
	dukglue_register_method(ctx, &jsGPU::draw_rectangle, "draw_rectangle");
	dukglue_register_method(ctx, &jsGPU::draw_text, "draw_text");
	dukglue_register_method(ctx, &jsGPU::draw_text_center, "draw_text_center");
	dukglue_register_method(ctx, &jsGPU::draw_text_wrap, "draw_text_wrap");
	dukglue_register_method(ctx, &jsGPU::draw_texture, "draw_texture");
	dukglue_register_method(ctx, &jsGPU::draw_texture_blend, "draw_texture_blend");
	dukglue_register_method(ctx, &jsGPU::draw_texture_flip, "draw_texture_flip");
	dukglue_register_method(ctx, &jsGPU::draw_texture_rotate, "draw_texture_rotate");
	dukglue_register_method(ctx, &jsGPU::draw_texture_scale, "draw_texture_scale");
	dukglue_register_method(ctx, &jsGPU::draw_texture_part, "draw_texture_part");
	dukglue_register_method(ctx, &jsGPU::end_draw, "end_draw");
	dukglue_register_method(ctx, &jsGPU::get_text_height_wrap, "get_text_height_wrap");
	dukglue_register_method(ctx, &jsGPU::get_text_height, "get_text_height");
	dukglue_register_method(ctx, &jsGPU::get_text_width, "get_text_width");
	dukglue_register_method(ctx, &jsGPU::free_texture, "free_texture");
	dukglue_register_method(ctx, &jsGPU::load_texture_bmp, "load_texture_bmp");
	dukglue_register_method(ctx, &jsGPU::load_texture_memory, "load_texture_memory");
	dukglue_register_method(ctx, &jsGPU::assign_texture_memory, "assign_texture_memory");
	dukglue_register_method(ctx, &jsGPU::load_texture_png, "load_texture_png");
	dukglue_register_method(ctx, &jsGPU::set_3D, "set_3D");
	dukglue_register_method(ctx, &jsGPU::set_screen_color, "set_screen_color");
	dukglue_register_method(ctx, &jsGPU::texture_width, "texture_width");
	dukglue_register_method(ctx, &jsGPU::texture_height, "texture_height");
	dukglue_register_method(ctx, &jsGPU::free_textures_all, "free_textures_all");

	dukglue_register_method(ctx, &jsGPU::texture_select, "texture_select");
	dukglue_register_method(ctx, &jsGPU::texture_select_part, "texture_select_part");
	dukglue_register_method(ctx, &jsGPU::texture_blend, "texture_blend");
	dukglue_register_method(ctx, &jsGPU::texture_scale, "texture_scale");
	dukglue_register_method(ctx, &jsGPU::texture_flip, "texture_flip");
	dukglue_register_method(ctx, &jsGPU::texture_rotate, "texture_rotate");
	dukglue_register_method(ctx, &jsGPU::texture_depth, "texture_depth");
	dukglue_register_method(ctx, &jsGPU::draw_selected, "draw_selected");

}

u32 GetTime()
{
	return (u32)osGetTime();
}

void SetBindings(duk_context* ctx)
{	
	dukglue_register_function(ctx, &RunFileInternal, "RunFile");

	dukglue_register_function(ctx, &print, "print");
	dukglue_register_function(ctx, &ConsoleCls, "clearConsole");
	dukglue_register_function(ctx, &GetScriptPath, "ScriptPath");
	dukglue_register_function(ctx, &ScriptResFolder, "ScriptRes");
	
	dukglue_register_function(ctx, &aptMainLoop, "aptMainLoop");
	dukglue_register_function(ctx, &svcSleepThread, "svcSleepThread");
	dukglue_register_function(ctx, &hidScanInput, "hidScanInput");
	dukglue_register_function(ctx, &hidKeysHeld, "hidKeysHeld");
	dukglue_register_function(ctx, &hidKeysDown, "hidKeysDown");
	
	dukglue_register_function(ctx, &gfxFlushBuffers, "gfxFlushBuffers");
	dukglue_register_function(ctx, &gfxSwapBuffers, "gfxSwapBuffers");
	dukglue_register_function(ctx, &MyGspWaitForVBlank, "gspWaitForVBlank");

	dukglue_register_function(ctx, &GetTime, "osGetTime");

	dukglue_register_function(ctx, &BottomScreenEnableConsole, "BottomScreenEnableConsole");
	dukglue_register_function(ctx, &BottomScreenDisableConsole, "BottomScreenDisableConsole");

	dukglue_register_function(ctx,&GETLIBFN(TouchScreen),"GetTouchScreen");
	dukglue_register_method(ctx, &TouchScreen::Refresh, "Refresh");
	dukglue_register_property(ctx, &TouchScreen::get_x, nullptr, "x");
	dukglue_register_property(ctx, &TouchScreen::get_y, nullptr, "y");

	SetGfxLibBindings(ctx);
	SetPP2DBindings(ctx);	
	SetSoundBindings(ctx);

	dukglue_register_function(ctx,&ReadAllBytes,"ReadAllBytes");
	dukglue_register_function(ctx,&FsFreeFile,"FsFreeFile");
}

void DisposeResources()
{
	SoundFreeAll(); 
	FREELIBFN(jsGPU)();
	FREELIBFN(TouchScreen)();
	pp2d_free_textures_all();
	FREELIBFN(GfxLib)();
	BottomScreenEnableConsole();
	FsFreeAll();	
}