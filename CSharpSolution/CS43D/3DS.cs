using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Bridge;
using Bridge.Html5;

namespace Lib3DS
{

    public static class Lib3DS
    {
        public const Int32 TOP_WIDTH = 400;
        public const Int32 BOTTOM_WIDTH = 320;
        public const Int32 SCREEN_HEIGHT = 240;

        public const byte GFX_TOP = 0;
        public const byte GFX_BOTTOM = 1;

        public const byte GFX_LEFT = 0;
        public const byte GFX_RIGHT = 1;
		
		public static void RunFile(string file) { Script.Call("RunFile",file); }
		
        public static void print(string str) { Script.Call("print", str); }
        public static void clearConsole() { Script.Call("clearConsole"); }
        public static string ScriptPath() { return Script.Call<string>("ScriptPath"); }
        public static string ScriptRes() { return Script.Call<string>("ScriptRes"); }

        public static bool aptMainLoop() { return Script.Call<bool>("aptMainLoop"); }
        public static void svcSleepThread(long time) { Script.Call("svcSleepThread", time); }
        public static void hidScanInput() { Script.Call("hidScanInput"); }
        public static Int32 hidKeysHeld() { return Script.Call<Int32>("hidKeysHeld"); }
        public static Int32 hidKeysDown() { return Script.Call<Int32>("hidKeysDown"); }
       
	    public static void gfxFlushBuffers() { Script.Call("gfxFlushBuffers"); }
        public static void gfxSwapBuffers() { Script.Call("gfxSwapBuffers"); }
        public static void gspWaitForVBlank() { Script.Call("gspWaitForVBlank"); }
		
        public static UInt32 osGetTime() { return Script.Call<UInt32>("osGetTime"); }
        
        public static void BottomScreenEnableConsole() { Script.Call("BottomScreenEnableConsole"); }
        public static void BottomScreenDisableConsole() { Script.Call("BottomScreenDisableConsole"); }
		
		public static TouchScreen GetTouchScreen() {return Script.Call<TouchScreen>("GetTouchScreen");}
		public static GfxLib GetGfxLib() {return Script.Call<GfxLib>("GetGfxLib");}		
		public static jsGPU GetJsGpu() {return Script.Call<jsGPU>("GetJsGpu");}
		
		public static WavFile SoundLoadWave(string path) { return Script.Call<WavFile>("SoundLoadWave", path); }
        public static void SoundStopChannel(byte chn) { Script.Call("SoundStopChannel",chn); }
        public static void SoundStopAllChannels() { Script.Call("SoundStopAllChannels"); }
        public static void SoundFreeWave(WavFile f) { Script.Call("SoundFreeWave",f); }
        public static void SoundFreeAll() { Script.Call("SoundFreeAll"); }
    }
    
    public enum GPU_FLIP_TYPE : byte
    {
         GPU_FLIP_VERTICAL = 1,
         GPU_FLIP_HORIZONTAL = 2,
         GPU_FLIP_BOTH = 3
    }

    public enum KEY : Int32 //Straight from ctrulib
    {
        A = 1 <<0,       //< A
        B = 1 <<1,       //< B
        SELECT = 1 <<2,       //< Select
        START = 1 <<3,       //< Start
        DRIGHT = 1 <<4,       //< D-Pad Right
        DLEFT = 1 <<5,       //< D-Pad Left
        DUP = 1 <<6,       //< D-Pad Up
        DDOWN = 1 <<7,       //< D-Pad Down
        R = 1 <<8,       //< R
        L = 1 <<9,       //< L
        X = 1 <<10,      //< X
        Y = 1 <<11,      //< Y
        ZL = 1 <<14,      //< ZL (New 3DS only)
        ZR = 1 <<15,      //< ZR (New 3DS only)
        TOUCH = 1 <<20,      //< Touch (Not actually provided by HID)
        CSTICK_RIGHT = 1 <<24, //< C-Stick Right (New 3DS only)
        CSTICK_LEFT = 1 <<25, //< C-Stick Left (New 3DS only)
        CSTICK_UP = 1 <<26, //< C-Stick Up (New 3DS only)
        CSTICK_DOWN = 1 <<27, //< C-Stick Down (New 3DS only)
        CPAD_RIGHT = 1 <<28,   //< Circle Pad Right
        CPAD_LEFT = 1 <<29,   //< Circle Pad Left
        CPAD_UP = 1 <<30,   //< Circle Pad Up
        CPAD_DOWN = 1 << 31,   //< Circle Pad Down

        // Generic catch-all directions
        UP = DUP | CPAD_UP,    //< D-Pad Up or Circle Pad Up
        DOWN = DDOWN | CPAD_DOWN,  //< D-Pad Down or Circle Pad Down
        LEFT = DLEFT | CPAD_LEFT,  //< D-Pad Left or Circle Pad Left
        RIGHT = DRIGHT | CPAD_RIGHT, //< D-Pad Right or Circle Pad Right
    }

    [External]
    [Namespace(false)]
    public class GfxLib
    {
		private GfxLib() {}
        public void setPixel(Int32 x, Int32 y, Int32 Color) { return; }
        public void setPixelRGB(Int32 x, Int32 y, byte R, byte G, byte B) { return; }
        public void EndFrame() { return; }
        public void ClearFb() { return; }
    }

    [External]
    [Namespace(false)]
    public class jsGPU
    {
		private jsGPU() {}
        public void begin_draw(byte target, byte side){}
        public void draw_on(byte target, byte side){}
        public void draw_rectangle(Int32 x, Int32 y, Int32 width, Int32 height, UInt32 color){}
        public void draw_text(float x, float y, float scaleX, float scaleY, UInt32 color, string text){}
        public void draw_text_center(byte target, float y, float scaleX, float scaleY, UInt32 color, string text){}
        public void draw_text_wrap(float x, float y, float scaleX, float scaleY, UInt32 color, float wrapX, string text){}
        public void draw_texture(UInt32 id, Int32 x, Int32 y){}
        public void draw_texture_blend(UInt32 id, Int32 x, Int32 y, UInt32 color){}
        public void draw_texture_flip(UInt32 id, Int32 x, Int32 y, byte fliptype){}
        public void draw_texture_rotate(UInt32 id, Int32 x, Int32 y, float angle){}
        public void draw_texture_scale(UInt32 id, Int32 x, Int32 y, float scaleX, float scaleY){}
        public void draw_texture_part(UInt32 id, Int32 x, Int32 y, Int32 xbegin, Int32 ybegin, Int32 width, Int32 height){}
        public void end_draw(){}
        public float get_text_height(string text, float scaleX, float scaleY){ return 0; }
        public float get_text_height_wrap(string text, float scaleX, float scaleY, Int32 wrapX) { return 0; }
        public float get_text_width(string text, float scaleX, float scaleY) { return 0; }
        public void free_texture(UInt32 id){}
        public UInt32 load_texture_bmp( string path){ return 0xFFFFFFFF; }
        public UInt32 load_texture_memory( byte[] buf, UInt32 width, UInt32 height) { return 0xFFFFFFFF; }
        public UInt32 assign_texture_memory(UInt32 tex, byte[] buf, UInt32 width, UInt32 height) { return 0xFFFFFFFF; }
        public UInt32 load_texture_memory( UInt32[] buf, UInt32 width, UInt32 height) { return 0xFFFFFFFF; }
        public UInt32 assign_texture_memory(UInt32 tex, UInt32[] buf, UInt32 width, UInt32 height) { return 0xFFFFFFFF; }
        public UInt32 load_texture_png( string path) { return 0xFFFFFFFF; }
        public void set_3D(bool enable){}
        public void set_screen_color(byte target, UInt32 color){}
        public UInt32 texture_width(UInt32 id) { return 0; }
        public UInt32 texture_height(UInt32 id) { return 0; }
        public void free_textures_all(){}

        public void texture_select(UInt32 id, Int32 x, Int32 y) { }
        public void texture_select_part(UInt32 id, Int32 x, Int32 y, Int32 xbegin, Int32 ybegin, Int32 width, Int32 height) { }
        public void texture_blend(UInt32 color) { }
        public void texture_scale(float scaleX, float scaleY) { }
        public void texture_flip(byte fliptype) { }
        public void texture_rotate(float angle) { }
        public void texture_depth(float depth) { }
        public void draw_selected() { }
    }

    [External]
    [Namespace(false)]
    public class WavFile
    {
        public void PlayOnChannel(byte channel, bool loop) { }
        public byte Play(bool loop) { return 0; }
    }
	
	[External]
	[Namespace(false)]
	public class TouchScreen
	{
		public UInt16 x {get;}
		public UInt16 y {get;}
		public void Refresh() {}
	}
}