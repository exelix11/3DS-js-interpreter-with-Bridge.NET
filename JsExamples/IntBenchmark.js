function g_int(x) {return x | 0;}

function main()
{
	var tmp;
	var time = osGetTime();
	for (var i = 0; i < 666000;i++)
	{
		tmp = (i/2);
	}
	time = osGetTime() - time;
	print("i/2 : " + time);
	
	time = osGetTime();
	for (var i = 0; i < 666000;i++)
	{
		tmp = Math.trunc(i/2);
	}	
	time = osGetTime() - time;
	print("Math.trunc(i/2) : " + time);
	
	time = osGetTime();
	for (var i = 0; i < 666000;i++)
	{
		tmp = ~~(i/2);
	}	
	time = osGetTime() - time;
	print("--(i/2) : " + time);
	
	time = osGetTime();
	for (var i = 0; i < 666000;i++)
	{
		tmp = (i/2) << 0;
	}	
	time = osGetTime() - time;
	print("(i/2) << 0 : " + time);
	
	time = osGetTime();
	for (var i = 0; i < 666000;i++)
	{
		tmp = (i/2) >> 0;
	}	
	time = osGetTime() - time;
	print("(i/2) >> 0 : " + time);
	
	time = osGetTime();
	for (var i = 0; i < 666000;i++)
	{
		tmp = (i/2) >>> 0;
	}	
	time = osGetTime() - time;
	print("(i/2) >>> 0 : " + time);
	
	time = osGetTime();
	for (var i = 0; i < 666000;i++)
	{
		tmp = (i/2) | 0;
	}	
	time = osGetTime() - time;
	print("(i/2) | 0 : " + time);
	
	time = osGetTime();
	for (var i = 0; i < 666000;i++)
	{
		tmp = g_int(i/2);
	}	
	time = osGetTime() - time;
	print("g_int(i/2) : " + time);
	
	var int = g_int;
	time = osGetTime();
	for (var i = 0; i < 666000;i++)
	{
		tmp = int(i/2);
	}	
	time = osGetTime() - time;
	print("int(i/2) : " + time);
	
	time = osGetTime();
	for (var i = 0; i < 666000;i++)
	{
		tmp = i >> 1;
	}	
	time = osGetTime() - time;
	print("i >> 1 : " + time);
	while (aptMainLoop())
	{
		hidScanInput();
		var kHeld = hidKeysDown();
		if (kHeld & KEY.START) break;
	}
}

main();