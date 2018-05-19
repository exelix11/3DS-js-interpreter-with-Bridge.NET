function loop()
{
	
	while (aptMainLoop())
	{
		hidScanInput();
		var kHeld = hidKeysDown();
		if (kHeld & KEY.START) break;
	}
}

function main()
{
	var f = ReadAllBytes(ScriptPath() + "/filetest.js");
	print("bytes: " + f[0] + " " + f[1] + " " + f[2] + " " + f[3]);
	loop();
	FsFreeFile(f);
	print("OK");
	loop();
}

main();