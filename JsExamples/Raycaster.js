const Colors = new Uint32Array(
[
	0xFF0000FF, 0xFF00007F, //Red
	0xFF00FF00, 0xFF007F00, //Green
	0xFFFF0000, 0xFF7F0000, //Blue
	0xFFFFFFFF, 0xFF7F7F7F, //White
	0xFF00FFFF, 0xFF007F7F, //Yellow
]);

var gpu = GetJsGpu();	

var deltaTime = 0;
var kDown = 0;
var kHeld = 0;
function main()
{
	var LastTime;
	while (aptMainLoop())
	{
		//LastTime = osGetTime();
		hidScanInput();
		kDown = hidKeysDown();		
		kHeld = hidKeysHeld();
		if (kDown & KEY.START) break;
		gpu.begin_draw(GFX_TOP,GFX_LEFT);
		RaycastLoop(TOP_WIDTH ,SCREEN_HEIGHT);
		MovePlayer();
		gpu.end_draw();
		//deltaTime = (LastTime - osGetTime());
	}
}

var worldMap=
[
  [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,5,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1],
  [1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,3,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1],
  [1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
];

var posX = 6, posY = 5;  //x and y start position
var dirX = -1, dirY = 0; //initial direction vector
var planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

function RaycastLoop(w,h){
	var abs = function(x){ return x < 0 ? -x : x;};
	var halfh = h/2;
	var map = worldMap;
	for(var x = 0; x < w; x++)
    {
      //calculate ray position and direction
      var cameraX = ((x * 2))/ w - 1; //x-coordinate in camera space
      //which box of the map we're in
      var mapX = (posX) | 0;
      var mapY = (posY) | 0;
	  var rayDirX = dirX + planeX * cameraX;
	  var rayDirY = dirY + planeY * cameraX;
      //length of ray from current position to next x or y-side
      var sideDistX;
      var sideDistY;

       //length of ray from one x or y-side to next x or y-side
      var deltaDistX = abs(1 / rayDirX);
      var deltaDistY = abs(1 / rayDirY);
      var perpWallDist;

      //what direction to step in x or y-direction (either +1 or -1)
      var stepX;
      var stepY;

      var hit = 0; //was there a wall hit?
      var side; //was a NS or a EW wall hit?
      //calculate step and initial sideDist
      if (rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
      }
      if (rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
      }
      //perform DDA
      while (hit == 0)
      {
        //jump to next map square, OR in x-direction, OR in y-direction
        if (sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        //Check if ray has hit a wall
        if (map[mapX][mapY] > 0) hit = 1;
      }
      //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
      if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
      else           perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

      //Calculate height of line to draw on screen
      var lineHeight = (((h / perpWallDist)/ 2) | 0);

      //calculate lowest and highest pixel to fill in current stripe

      //choose wall color
      var color = (((map[mapX][mapY] - 1) * 2) | 0) + side;

      //draw the pixels of the stripe as a vertical line
	  gpu.draw_rectangle(x,halfh - lineHeight,1,(lineHeight * 2 ) | 0,Colors[color]);
    }
}

const moveSpeed = 0.2;
const rotSpeed = 0.12; //the constant value is in radians/msecond
function MovePlayer()
{
    //speed modifiers
	if (kHeld == 0) return;
    //move forward if no wall in front of you
    if (kHeld & KEY.UP)
    {
      
      if(worldMap[(posX + dirX * moveSpeed) | 0][(posY)| 0] == false) posX += dirX * moveSpeed;
      if(worldMap[(posX)| 0][(posY + dirY * moveSpeed)| 0] == false) posY += dirY * moveSpeed;
    }
    else if (kHeld & KEY.DOWN)
    {
      if(worldMap[(posX - dirX * moveSpeed)| 0][(posY)| 0] == false) posX -= dirX * moveSpeed;
      if(worldMap[(posX)| 0][(posY - dirY * moveSpeed)| 0] == false) posY -= dirY * moveSpeed;
    }
	
    if (kHeld & KEY.LEFT)
    {
	  var sin = Math.sin(rotSpeed);
	  var cos = Math.cos(rotSpeed);
      //both camera direction and camera plane must be rotated
      var oldDirX = dirX;
      dirX = dirX * cos - dirY * sin;
      dirY = oldDirX * sin + dirY * cos;
      var oldPlaneX = planeX;
      planeX = planeX * cos - planeY * sin;
      planeY = oldPlaneX * sin + planeY * cos;
    }
    else if (kHeld & KEY.RIGHT)
    {
      //both camera direction and camera plane must be rotated
	  var sin = Math.sin(-rotSpeed);
	  var cos = Math.cos(-rotSpeed);
      var oldDirX = dirX;
      dirX = dirX * cos - dirY * sin;
      dirY = oldDirX * sin + dirY * cos;
      var oldPlaneX = planeX;
      planeX = planeX * cos - planeY * sin;
      planeY = oldPlaneX * sin + planeY * cos;
    }
}

main();