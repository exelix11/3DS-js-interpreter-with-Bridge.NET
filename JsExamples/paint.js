var arr = new Uint32Array(BOTTOM_WIDTH*SCREEN_HEIGHT);

var CurSize = 3;
var color = 0xFFFFFFFF;

function cursor(x,y)
{
	for (var i = 0; i < CurSize;i++)
	{
		for (var j = 0; j < CurSize; j++)
			arr[(j + y) * BOTTOM_WIDTH + x + i] = color;
	}
}

function setPixel(x,y)
{
	arr[y * BOTTOM_WIDTH + x] = color;
}

function line(x0, y0, x1, y1){
	var abs = function(x) {return x < 0 ? -x : x;}
   var dx = abs(x1-x0);
   var dy = abs(y1-y0);
   var sx = (x0 < x1) ? 1 : -1;
   var sy = (y0 < y1) ? 1 : -1;
   var err = dx-dy;

   while(true){
     cursor(x0,y0);  

     if ((x0==x1) && (y0==y1)) break;
     var e2 = 2*err;
     if (e2 >-dy){ err -= dy; x0  += sx; }
     if (e2 < dx){ err += dx; y0  += sy; }
   }
}

function main()
{
	var oldX = 0,oldY = 0;
	var gpu = GetJsGpu();	
	BottomScreenDisableConsole();
	var texture = gpu.load_texture_memory(arr,BOTTOM_WIDTH,SCREEN_HEIGHT);
	var touch = GetTouchScreen();
	while (aptMainLoop())
	{
		hidScanInput();
		kDown = hidKeysDown();		
		kHeld = hidKeysHeld();
		if (kDown & KEY.START) break;
		else if (kDown & KEY.LEFT) 
		{
			if (CurSize != 1) CurSize--;
		}
		else if (kDown & KEY.RIGHT)
		{
			if (CurSize != 10) CurSize++;
		}
		if (kHeld & KEY.L || kHeld & KEY.R) 
		{
			color = 0;
		}
		else color = 0xFFFFFFFF;
		touch.Refresh();
		if (touch.x != 0 && touch.y != 0)
		{
			if (oldX != 0 && oldY != 0)
			{
				line(touch.x,touch.y,oldX,oldY);
			}
			else 
				cursor(touch.x,touch.y);
			oldX = touch.x;
			oldY = touch.y;
			gpu.assign_texture_memory(texture,arr,BOTTOM_WIDTH,SCREEN_HEIGHT);
		}
		else 
		{
			oldX = 0;
			oldY = 0;
		}		
		gpu.begin_draw(GFX_BOTTOM,GFX_LEFT);
		gpu.draw_texture(texture,0,0);
		gpu.draw_on(GFX_TOP,GFX_LEFT);
		gpu.draw_text(0,0,1,1,0xFFFFFFFF,"POS: " + touch.x + " " + touch.y );
		gpu.end_draw();
	}
}

main();