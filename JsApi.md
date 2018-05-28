# 3DS JavaScript api
These are the bindings implemented in the current version of the js interpreter.
The js interpreter is Duktape, for more informations on the supported js apis and optimizing performances check the following links:
http://duktape.org/guide.html
http://wiki.duktape.org/Performance.html

## JsTest-specific apis
print(x)
>Prints a string to the console (if enabled)
---
clearConsole()
> Clears the console (if enabled)
---
ScriptPath()
>Returns the path of the current script
---
ScriptRes()
>Returns the resources folder of the current script (eg. sdmc://js//script.js-res/)
---
BottomScreenDisableConsole()
>Disables console on the bottom screen (to use it as render target for gpu)
---
BottomScreenEnableConsole()
>Enables the console on the bottom screen (unrecommended, might break things)
---
GetTouchScreen()
> returns an object containing the current position on the touch screen, you should use this only once, if you need to get again the position use the Refresh() method
```
var t = GetTouchScreen();
var X = t.x;
var Y = t.y;
t.Refresh();
```
---
## Ctrulib bindings
aptMainLoop()
svcSleepThread(time)
hidScanInput()
hidKeysHeld()
hidKeysDown()
gfxFlushBuffers()
gfxSwapBuffers()
gspWaitForVBlank()
## Gpu
This is powered by pp2d
GetJsGpu()
> returns a JsGpu object, use this only once.
---
>Target can be either GFX_TOP or GFX_BOTTOM
Side can be either GFX_LEFT or GFX_RIGHT (only for top screen, use always GFX_LEFT for bottom screen)

>Texture loading functions return an integer ID of the loaded texture

Typed definitions:
```
gfx.begin_draw(byte target, byte side)
gfx.draw_on(byte target, byte side)
gfx.end_draw()
gfx.draw_rectangle(Int32 x, Int32 y, Int32 width, Int32 height, UInt32 color)
gfx.draw_text(float x, float y, float scaleX, float scaleY, UInt32 color, string text)
gfx.draw_text_center(byte target, float y, float scaleX, float scaleY, UInt32 color, string text)
gfx.draw_text_wrap(float x, float y, float scaleX, float scaleY, UInt32 color, float wrapX, string text)
gfx.draw_texture(UInt32 id, Int32 x, Int32 y)
gfx.draw_texture_blend(UInt32 id, Int32 x, Int32 y, UInt32 color)
gfx.draw_texture_flip(UInt32 id, Int32 x, Int32 y, byte fliptype)
gfx.draw_texture_rotate(UInt32 id, Int32 x, Int32 y, float angle)
gfx.draw_texture_scale(UInt32 id, Int32 x, Int32 y, float scaleX, float scaleY)
gfx.draw_texture_part(UInt32 id, Int32 x, Int32 y, Int32 xbegin, Int32 ybegin, Int32 width, Int32 height)
gfx.get_text_height(string text, float scaleX, float scaleY)
gfx.get_text_height_wrap(string text, float scaleX, float scaleY, Int32 wrapX) 
gfx.get_text_width(string text, float scaleX, float scaleY)
gfx.free_texture(UInt32 id)
gfx.load_texture_bmp( string path)
gfx.load_texture_memory( byte[] buf, UInt32 width, UInt32 height)
gfx.assign_texture_memory(UInt32 tex, byte[] buf, UInt32 width, UInt32 height) 
gfx.load_texture_memory( UInt32[] buf, UInt32 width, UInt32 height) 
gfx.assign_texture_memory(UInt32 tex, UInt32[] buf, UInt32 width, UInt32 height) 
gfx.load_texture_png( string path)
gfx.set_3D(bool enable)
gfx.set_screen_color(byte target, UInt32 color)
gfx.texture_width(UInt32 id) 
gfx.texture_height(UInt32 id) 
gfx.free_textures_all()

gfx.texture_select(UInt32 id, Int32 x, Int32 y) 
gfx.texture_select_part(UInt32 id, Int32 x, Int32 y, Int32 xbegin, Int32 ybegin, Int32 width, Int32 height) 
gfx.texture_blend(UInt32 color) 
gfx.texture_scale(float scaleX, float scaleY) 
gfx.texture_flip(byte fliptype) 
gfx.texture_rotate(float angle) 
gfx.texture_depth(float depth) 
gfx.draw_selected() 
```
---
## Sound
SoundStopChannel(channelNo)
> Stops the wave playing on channelNo (channels range from 0 to 22)
---
SoundStopAllChannels()
> Stops every sound
---
SoundLoadWave(path)
>returns an object containing the specified .wav file
```
var wave = SoundLoadWave(ScriptRes() + "/Sound.wav");
wave.Play(true); //Plays and loops the wave on a free channel
wave.Play(false); //Plays once the wave on a free channel
wave.PlayOnChannel(0,true); //Plays and loops the wave on channel 0
wave.PlayOnChannel(0,false); //Plays once the wave on channel 0
```
---
SoundFreeWave(wave)
> Removes a loaded wave file from memory
---
SoundFreeAll()
> Removes every wave file from memory
---
## FileSystem
To read a file from the resources folder use:
``` ScriptRes() + "/" + filename```
ReadAllBytes(path)
> Reads a file, returns a Uint8Array
---
FsFreeFile(Uint8Array)
> Unloads a loaded file from memory, should be called when a file from ReadAllBytes isn't needed anymore
---
## Raw Framebuffer
Don't use both these and gpu functions together !!
GetGfxLib()
> Gets the gfxLib object (top screen only)
```
var gfx = GetGfxLib();
gfx.setPixel(x,y,Color); //draws a pixel on the framebuffer hex color format: 0xFFFFFF = white
gfx.setPixelRGB(x,y,R,G,B); //draws a pixel on the framebuffer
gfx.EndFrame(); //Flushes and swaps buffers (ctrulib)
gfx.ClearFb(); //Fills the screen with black
```
## Beta/Unimplemented
RunFile(path)
> Runs a script, without updating the script res folder, ScriptRes() will return the resources folder of the scripts that launched it
---
osGetTime()
> Doesn't return the correct time (c++ long > js float), can be used to count seconds.
---
