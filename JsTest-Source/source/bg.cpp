#include "bg.h"
#include "pp2d/pp2d.h"

const double PI = 3.141592653589793;
const double freq = 2 * PI / TOP_WIDTH;
int bgTileCount = 0;
int xbg = 0;
bool BgWave = false;

void InitBgWave(int k = 1)
{
	u32 *buf = new u32[TOP_WIDTH * 32];
	memset(buf,0,TOP_WIDTH * 32*4);
	for (int x = 0; x < TOP_WIDTH; x++)
	{
		int y = (int)(sin((x - xbg ) * freq * k ) * 16 + 16); 
		buf[y * TOP_WIDTH + x] = 0xFFFFFFFF;
		while (y <= 32) buf[++y * TOP_WIDTH + x] = 0xFFFFFFFF;
	}
	pp2d_load_texture_memory(0,buf,TOP_WIDTH,32);
	delete[] buf;
    BgWave = true;
    xbg = 0;
}

void DrawBg()
{	
    if (BgWave)
    {
        pp2d_draw_rectangle(0,SCREEN_HEIGHT - 32, TOP_WIDTH,32,0xFFFFFFFF);
	    pp2d_draw_texture(0, xbg,SCREEN_HEIGHT - 64);
	    pp2d_draw_texture(0, xbg - TOP_WIDTH,SCREEN_HEIGHT - 64);
	    if (xbg >= TOP_WIDTH) 
    		xbg = 0; 
	    else    
    		xbg += 2;
    }
}

void ClearBgWave()
{
    BgWave = false;
    pp2d_free_texture(0);
}
