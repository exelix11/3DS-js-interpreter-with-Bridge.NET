#pragma once
#include <stdio.h>
#include <iostream>
#include <3ds.h>
#include <string.h>

//Raw framebuffer drawing lib
class GfxLib
{
	public:	
	u8* fb;
	
	GfxLib();
	~GfxLib();
	void setPixel(u16 x, u16 y, u32 COl);
	void setPixelRGB(u16 x, u16 y, u8 R,u8 G,u8 B);
	void EndFrame();
	void InitFb();	
	void ClearFb();
};

#include "FastLibDefine.h"
EXTERN_LIBDEFINE(GfxLib);