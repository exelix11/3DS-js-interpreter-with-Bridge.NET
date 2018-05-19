function rand(min,max)
{
	return Math.floor(Math.random() * max + min);
}

var gpu = GetJsGpu();
var x = 0, y = 0;
var xSpeed = rand(1,5), ySpeed = rand(3,10);
var texture = gpu.load_texture_png(ScriptRes() + "/troll.png");
var w = gpu.texture_width(0);
var h = gpu.texture_height(0);
var kDown = 0;
var testLoop = SoundLoadWave(ScriptRes() + "/TestLoop.wav");
testLoop.Play(false);
while (aptMainLoop())
{
	hidScanInput();
	kDown = hidKeysDown();
	if (kDown & KEY.A) 
	{
		xSpeed = rand(1,5)
		ySpeed = rand(3,10);
	}
	else if (kDown & KEY.START) break;
	x+=xSpeed;
	y+=ySpeed;
	gpu.begin_draw(GFX_TOP,GFX_LEFT);
	if (xSpeed > 0) gpu.draw_texture_flip(texture,x,y,2);
	else gpu.draw_texture(0,x,y);
	gpu.end_draw();
	if (x >= TOP_WIDTH - w || x <= 0) xSpeed =-xSpeed;
	if (y >= SCREEN_HEIGHT - h || y <= 0) ySpeed =-ySpeed;
	gspWaitForVBlank();
}