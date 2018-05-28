#include "jsGPU.h"
#include <dukglue/dukglue.h>

LIBDEFINE(jsGPU)

jsGPU::jsGPU()
{
    //pp2d_init();
}

extern duk_context *mainCtx; //from main.cpp
jsGPU::~jsGPU()
{
    if (InDraw) end_draw();
    printf("jsGPU destroyed\n");
}

u32 jsGPU::getFreeTextureID()
{
    for (int i = 0;i < MAX_TEXTURES; i++) 
    {
        if (!pp2d_get_texture_allocated(i)) 
        {
            return i;
        }
    }
    return 0xFFFFFFFF;
}

gfxScreen_t jsGPU::screenFromId(int id) {return id == 1 ? GFX_BOTTOM : GFX_TOP;}
gfx3dSide_t jsGPU::sideFromId(int id) {return id == 1 ? GFX_RIGHT : GFX_LEFT;}

void jsGPU::begin_draw(u8 target, u8 side)
{
    pp2d_begin_draw(screenFromId(target), sideFromId(side));
    InDraw = true;
}

void jsGPU::draw_on(u8 target, u8 side)
{
    pp2d_draw_on(target == 1 ? GFX_BOTTOM : GFX_TOP, side == 1 ? GFX_RIGHT : GFX_LEFT);
}

void jsGPU::draw_rectangle(s32 x, s32 y, s32 width, s32 height, u32 color)
{
    pp2d_draw_rectangle(x,y,width,height,color);
}

void jsGPU::draw_text(float x, float y, float scaleX, float scaleY, u32 color, const char* text)
{
    pp2d_draw_text(x,y,scaleX,scaleY,color,text);
}

void jsGPU::draw_text_center(u8 target, float y, float scaleX, float scaleY, u32 color, const char* text)
{
    pp2d_draw_text_center(screenFromId(target),y,scaleX,scaleY,color,text);
}

void jsGPU::draw_text_wrap(float x, float y, float scaleX, float scaleY, u32 color, float wrapX, const char* text)
{
    pp2d_draw_text_wrap(x,y,scaleX,scaleY,color,wrapX,text);
}

void jsGPU::draw_texture(u32 id, s32 x, s32 y)
{
    pp2d_draw_texture((size_t)id,x,y);
}

void jsGPU::draw_texture_blend(u32 id, s32 x, s32 y, u32 color)
{
    pp2d_draw_texture_blend((size_t)id,x,y,color);
}

void jsGPU::draw_texture_flip(u32 id, s32 x, s32 y, u8 fliptype)
{
    flipType t = NONE;
    switch (fliptype) 
    {
        case 1: 
            t = VERTICAL;
            break;
        case 2:
            t = HORIZONTAL;
            break;
        case 3:
            t = BOTH;
            break;
    }
    pp2d_draw_texture_flip((size_t)id,x,y,t);

}

void jsGPU::draw_texture_rotate(u32 id, s32 x, s32 y, float angle)
{
    pp2d_draw_texture_rotate((size_t)id,x,y,angle);
}

void jsGPU::draw_texture_scale(u32 id, s32 x, s32 y, float scaleX, float scaleY)
{
    pp2d_draw_texture_scale((size_t)id,x,y,scaleX,scaleY);
}

void jsGPU::draw_texture_part(u32 id, s32 x, s32 y, s32 xbegin, s32 ybegin, s32 width, s32 height)
{
    pp2d_draw_texture_part((size_t)id,x,y,xbegin,ybegin,width,height);
}

void jsGPU::end_draw(void)
{
    pp2d_end_draw();
    InDraw = false;
}

float jsGPU::get_text_height(const char* text, float scaleX, float scaleY)
{
    return pp2d_get_text_height(text,scaleX,scaleY);
}

float jsGPU::get_text_height_wrap(const char* text, float scaleX, float scaleY, s32 wrapX)
{
    return pp2d_get_text_height_wrap(text,scaleX,scaleY,wrapX);
}

float jsGPU::get_text_width(const char* text, float scaleX, float scaleY)
{
    return pp2d_get_text_width(text,scaleX,scaleY);
}

void jsGPU::free_texture(u32 id)
{
    pp2d_free_texture((size_t)id);
}

u32 jsGPU::load_texture_bmp(const char* path)
{
    u32 id = getFreeTextureID();
    pp2d_load_texture_bmp((size_t)id,path);    
    return id;
}

u32 jsGPU::load_texture_memory(const DukBuffer& pixelData, u32 w, u32 h)
{
    u32 id = getFreeTextureID();
    pp2d_load_texture_memory((size_t)id,pixelData.ptr,w,h);    
    return id;
}

u32 jsGPU::assign_texture_memory(u32 ID, const DukBuffer& pixelData, u32 w, u32 h)
{
    pp2d_load_texture_memory((size_t)ID,pixelData.ptr,w,h); 
    return ID;
}

u32 jsGPU::load_texture_png(const char* path)
{
    u32 id = getFreeTextureID();
    pp2d_load_texture_png((size_t)id,path);     
    return id;  
}

void jsGPU::set_3D(bool enable)
{
    pp2d_set_3D(enable ? 1 : 0);
}

void jsGPU::set_screen_color(u8 target, u32 color)
{
    pp2d_set_screen_color(screenFromId(target),color); 
}

u32 jsGPU::texture_width(u32 id) {return pp2d_get_texture_width((size_t)id);}

u32 jsGPU::texture_height(u32 id) {return pp2d_get_texture_height((size_t)id);}

void jsGPU::free_textures_all() {pp2d_free_textures_all();}

void jsGPU::texture_select(u32 id, s32 x, s32 y) {pp2d_texture_select((size_t)id, x, y);}

void jsGPU::texture_select_part(u32 id, s32 x, s32 y, s32 xbegin, s32 ybegin,s32 width, s32 height)
    {pp2d_texture_select_part((size_t)id, x, y, xbegin, ybegin, width, height);}

void jsGPU::texture_blend(u32 color) {pp2d_texture_blend(color);}

void jsGPU::texture_scale(float scaleX, float scaleY) { pp2d_texture_scale(scaleX, scaleY);}

void jsGPU::texture_flip(u8 fliptype)
{
    flipType t = NONE;
    switch (fliptype) 
    {
        case 1: 
            t = VERTICAL;
            break;
        case 2:
            t = HORIZONTAL;
            break;
        case 3:
            t = BOTH;
            break;
    }
    pp2d_texture_flip(t);
}

void jsGPU::texture_rotate(float angle){pp2d_texture_rotate(angle);}

void jsGPU::texture_depth(float depth) {pp2d_texture_depth(depth);}

void jsGPU::draw_selected(void) { pp2d_texture_draw();}
