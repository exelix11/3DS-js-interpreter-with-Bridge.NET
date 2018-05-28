#pragma once
#include <stdio.h>
#include <iostream>
#include <3ds.h>
#include <string.h>
#include <vector>
#include "../pp2d/pp2d.h"
#include <dukglue/dukvalue.h>

class jsGPU
{
    private:
        u32 getFreeTextureID();
        gfxScreen_t screenFromId(int id);
        gfx3dSide_t sideFromId(int id);
        bool InDraw = false;
    public: 
    jsGPU();
    ~jsGPU();
/**
 * @brief Starts a new frame on the specified screen
 * @param if target = 1 bottom screen, else top, if side = 1 right else left
 */
    void begin_draw(u8 target, u8 side);

/**
 * @brief Changes target screen to the specified target
 * @param target GFX_TOP or GFX_BOTTOM
 */
    void draw_on(u8 target, u8 side);

/**
 * @brief Draw a rectangle
 * @param x of the top left corner
 * @param y of the top left corner
 * @param width on the rectangle
 * @param height of the rectangle
 * @param color RGBA8 to fill the rectangle
 */
    void draw_rectangle(s32 x, s32 y, s32 width, s32 height, u32 color);

/**
 * @brief Prints a char pos32er
 * @param x position to start drawing
 * @param y position to start drawing
 * @param scaleX multiplier for the text width
 * @param scaleY multiplier for the text height
 * @param color RGBA8 the text will be drawn
 * @param text to be prs32ed on the screen
 */
    void draw_text(float x, float y, float scaleX, float scaleY, u32 color, const char* text);

/**
 * @brief Prints a char pos32er in the middle of the target screen
 * @param target screen to draw the text to
 * @param y position to start drawing
 * @param scaleX multiplier for the text width
 * @param scaleY multiplier for the text height
 * @param color RGBA8 the text will be drawn
 * @param text to be prs32ed on the screen
 */
    void draw_text_center(u8 target, float y, float scaleX, float scaleY, u32 color, const char* text);

/**
 * @brief Prints a char pos32er in the middle of the target screen
 * @param x position to start drawing
 * @param y position to start drawing
 * @param scaleX multiplier for the text width
 * @param scaleY multiplier for the text height
 * @param color RGBA8 the text will be drawn
 * @param wrapX wrap width
 * @param text to be prs32ed on the screen
 */
    void draw_text_wrap(float x, float y, float scaleX, float scaleY, u32 color, float wrapX, const char* text);

/**
 * @brief Prints a texture
 * @param id of the texture 
 * @param x position on the screen to draw the texture
 * @param y position on the screen to draw the texture
 */
    void draw_texture(u32 id, s32 x, s32 y);

/**
 * @brief Prints a texture with color modulation
 * @param id of the texture 
 * @param x position on the screen to draw the texture
 * @param y position on the screen to draw the texture
 * @param color RGBA8 to modulate the texture with
 */
    void draw_texture_blend(u32 id, s32 x, s32 y, u32 color);

/**
 * @brief Prints a flipped texture
 * @param id of the texture 
 * @param x position on the screen to draw the texture
 * @param y position on the screen to draw the texture
 * @param fliptype HORIZONTAL, VERTICAL or BOTH
 */

    void draw_texture_flip(u32 id, s32 x, s32 y, u8 fliptype);

/**
 * @brief Prints a rotated texture
 * @param id of the texture 
 * @param x position on the screen to draw the texture
 * @param y position on the screen to draw the texture
 * @param angle in degrees to rotate the texture
 */
    void draw_texture_rotate(u32 id, s32 x, s32 y, float angle);

/**
 * @brief Prints a texture with a scale factor
 * @param id of the texture 
 * @param x position on the screen to draw the texture
 * @param y position on the screen to draw the texture
 * @param scaleX width scale factor
 * @param scaleY height scale factor
 */
    void draw_texture_scale(u32 id, s32 x, s32 y, float scaleX, float scaleY);

/**
 * @brief Prints a portion of a texture
 * @param id of the texture 
 * @param x position on the screen to draw the texture
 * @param y position on the screen to draw the texture
 * @param xbegin position to start drawing
 * @param ybegin position to start drawing
 * @param width to draw from the xbegin position 
 * @param height to draw from the ybegin position
 */
    void draw_texture_part(u32 id, s32 x, s32 y, s32 xbegin, s32 ybegin, s32 width, s32 height);

/// Ends a frame
    void end_draw(void);

/**
 * @brief Calculates a char pos32er height
 * @param text char pos32er to calculate the height of
 * @param scaleX multiplier for the text width 
 * @param scaleY multiplier for the text height
 * @return height the text will have if rendered in the supplied conditions
 */
    float get_text_height(const char* text, float scaleX, float scaleY);

/**
 * @brief Calculates a char pos32er height
 * @param text char pos32er to calculate the height of
 * @param scaleX multiplier for the text width 
 * @param scaleY multiplier for the text height
 * @param wrapX wrap width
 * @return height the text will have if rendered in the supplied conditions
 */
    float get_text_height_wrap(const char* text, float scaleX, float scaleY, s32 wrapX);

/**
 * @brief Calculates a char pos32er width
 * @param text char pos32er to calculate the width of
 * @param scaleX multiplier for the text width 
 * @param scaleY multiplier for the text height
 * @return width the text will have if rendered in the supplied conditions
 */
    float get_text_width(const char* text, float scaleX, float scaleY);

/**
 * @brief Frees a texture
 * @param id of the texture to free
 */
    void free_texture(u32 id);

/**
 * @brief Loads a texture from a bmp file
 * @param id of the texture
 * @param path where the bmp file is located
 */
    u32 load_texture_bmp(const char* path);

/**
 * @brief Loads a texture from a a buffer in memory
 * @param id of the texture 
 * @param buf buffer where the texture is stored
 * @param width of the texture
 * @param height of the texture
 */
    u32 load_texture_memory(const DukBuffer& pixelData, u32 w, u32 h);

    u32 assign_texture_memory(u32 ID, const DukBuffer& pixelData, u32 w, u32 h);
/**
 * @brief Loads a texture from a png file
 * @param id of the texture 
 * @param path where the png file is located 
 */
    u32 load_texture_png(const char* path);

/**
 * @brief Enables 3D service
 * @param enable integer
 */
    void set_3D(bool enable);

/**
 * @brief Sets a background color for the specified screen
 * @param target GFX_TOP or GFX_BOTTOM
 * @param color ABGR8 which will be the background one
 */
    void set_screen_color(u8 target, u32 color);

    u32 texture_width(u32 id);

    u32 texture_height(u32 id);

    void free_textures_all();


/**
 * @brief Inits a texture to be drawn
 * @param id of the texture
 * @param x to draw the texture at
 * @param y to draw the texture at
 */
void texture_select(u32 id, s32 x, s32 y);

/**
 * @brief Inits a portion of a texture to be drawn
 * @param id of the texture 
 * @param x position on the screen to draw the texture
 * @param y position on the screen to draw the texture
 * @param xbegin position to start drawing
 * @param ybegin position to start drawing
 * @param width to draw from the xbegin position 
 * @param height to draw from the ybegin position
 */
void texture_select_part(u32 id, s32 x, s32 y, s32 xbegin, s32 ybegin, s32 width,s32 height);

/**
 * @brief Modulates a texture with a color
 * @param color to modulate the texture
 */
void texture_blend(u32 color);

/**
 * @brief Scales a texture
 * @param scaleX width scale factor
 * @param scaleY height scale factor
 */
void texture_scale(float scaleX, float scaleY);

/**
 * @brief Flips a texture
 * @param fliptype HORIZONTAL, VERTICAL or BOTH
 */
void texture_flip(u8 fliptype);

/**
 * @brief Rotates a texture
 * @param angle in degrees to rotate the texture
 */
void texture_rotate(float angle);

/**
 * @brief Sets the depth of a texture
 * @param depth factor of the texture
 */
void texture_depth(float depth);

/// Renders a texture
void draw_selected(void);
};

#include "FastLibDefine.h"
EXTERN_LIBDEFINE(jsGPU);
