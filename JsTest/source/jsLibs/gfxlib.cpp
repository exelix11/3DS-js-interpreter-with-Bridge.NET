#include "gfxlib.h"
#include <dukglue/dukglue.h>

LIBDEFINE(GfxLib);
	
GfxLib::GfxLib()
{
	InitFb();
}

GfxLib::~GfxLib(){
	gfxSetDoubleBuffering(GFX_TOP, true);
}

void GfxLib::setPixel(u16 x, u16 y, u32 COl) 
{
	setPixelRGB(x,y,((u8*)&COl)[0],((u8*)&COl)[1],((u8*)&COl)[2]);
}

void GfxLib::setPixelRGB(u16 x, u16 y, u8 R,u8 G,u8 B)
{
	u32 index = 3 * (240 - y + (x - 1) * 240);
	fb[index] = R;
	fb[++index] = G;
	fb[++index] = B;
}

void GfxLib::EndFrame()
{
	gfxFlushBuffers();
	gfxSwapBuffers();
}

void GfxLib::InitFb()
{
	gfxSetDoubleBuffering(GFX_TOP, false);
	fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);	
	ClearFb();		
}

void GfxLib::ClearFb()
{
	memset(fb, 0, 400*240*3);
}
