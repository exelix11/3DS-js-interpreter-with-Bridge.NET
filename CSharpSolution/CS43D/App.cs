using Bridge;
using Bridge.Html5;
using Newtonsoft.Json;
using System;
using Lib3DS;
using l3d = Lib3DS.Lib3DS;

namespace CS43D
{
    public class App
    {
        class GameObj
        {
            public int x, y, w, h;
            public bool alive;
            public bool collide(GameObj o)
            {
                return !((x > o.x + o.w - 1 || // Is box 1 on the right side of box 2? 
                          y > o.y + o.h - 1 || // Is box 1 under box 2?                
                          o.x > x + w - 1 || // Is box 2 on the right side of box 1? 
                          o.y > y + h - 1));// Is box 2 under box 1?                
            }

            public virtual void Draw() { }
            public virtual void Frame() { }
            public virtual void CheckProjHit(GameObj proj) { }
            public override string ToString()
            {
                return "[GenericObj]";
            }
        }

        class Barrier : GameObj
        {
            public override void Draw()
            {
                gpu.draw_rectangle(x, y, w, h, 0xFFFFFFFF);
            }

            public override void CheckProjHit(GameObj proj)
            {
                if (!alive) return;
                if (collide(proj))
                {
                    proj.alive = false;
                    w -= 16;
                    x += 8;
                    if (w <= 0)
                    {
                        alive = false;
                    }
                    return;
                }
            }

            public override string ToString()
            {
                return "[Barrier]";
            }
        }

        class Enemy : GameObj
        {
            public bool altSpr = false;

            public override void Draw()
            {
                if (x < -32 || x > l3d.TOP_WIDTH) return;
                if (altSpr == UseAltSpr) 
                    gpu.draw_texture(textures[0],x, y);
                else
                    gpu.draw_texture(textures[1], x, y);
            }

            public override void CheckProjHit(GameObj proj)
            {
                if (!alive || x < 32) return;
                if (collide(proj))
                {
                    explosion.Play(false);
                    ++DefeatedEnemies;
                    proj.alive = false;
                    alive = false;
                    return;
                }
            }

            public override void Frame()
            {
                x += EnemyMoveSpeed;
                if (x > l3d.TOP_WIDTH)
                {
                    x = -32;
                    y += 24;
                    if (y >= player.y) { player.alive = false; }
                }
                if (FrameEnemyShoot > 30 && x > 0)
                {
                    if (Rand(0.006))
                    {
                        Objects.Push(new Proj { x = x + (w >> 1) - 4, y = y, w = 8, h = 8, alive = true, speed = 10, hitType = typeof(Player) });
                        FrameEnemyShoot = 0;
                    }
                }
            }
            public override string ToString()
            {
                return "[Enemy]";
            }
        }

        class Proj : GameObj
        {
            public int speed;
            public Type hitType; //Enemy
            public override void Draw()
            {
                gpu.draw_rectangle(x, y, w, h, 0xFFFFFFFF);
            }

            public override void Frame()
            {
                y += speed;
                if (y < -8 || y > l3d.SCREEN_HEIGHT)
                {
                    alive = false;
                    return;
                }
                for (int i = 0; i < Objects.Length; i++)
                {
                    var otherType = Objects[i].GetType();
                    if (otherType != hitType && otherType != typeof(Barrier)) continue;
                    Objects[i].CheckProjHit(this);
                    if (!alive) break;
                }
            }

            public override string ToString()
            {
                return "[Proj]";
            }
        }

        class Player : GameObj
        {
            public override void Draw()
            {
                gpu.draw_rectangle(x, y, w, h, 0xFFFFFFFF);
            }

            public override void Frame()
            {
                if (Key(keyHeld, KEY.LEFT)) { if (x >= 4) x -= 4; else x = 0; }
                else if (Key(keyHeld, KEY.RIGHT)) { if (x < l3d.TOP_WIDTH - 4 - w ) x += 4; else x = l3d.TOP_WIDTH - w; }
                if (Key(KeyDown, KEY.A) && projCoolDown > 8)
                {
                    projCoolDown = 0;
                    shoot.Play(false);
                    Objects.Push(new Proj { x = x + (w >> 1) - 4, y = y, w = 8, h = 8, alive = true, speed = -10, hitType = typeof(Enemy) });
                }
            }

            public override void CheckProjHit(GameObj proj)
            {
                if (collide(proj)) alive = false;
            }
            public override string ToString()
            {
                return "[Player]";
            }
        }

        static uint[] textures = new uint[2];
        static jsGPU gpu = null;

        static int DefeatedEnemies = 0;

        static GameObj[] Objects = new GameObj[0];
        static Player player;

        static int LastSpawnedX;
        static bool EnAltSpr = false;

        static void InitGame() 
        {
            DefeatedEnemies = 0;
            Objects = new GameObj[0];
            player = new Player { x = l3d.TOP_WIDTH / 2 - 16, y = l3d.SCREEN_HEIGHT - 17, w = 32, h = 16, alive = true };
            Objects.Push(player);

            int x = -32;
            while (x < l3d.TOP_WIDTH)
            {
                Objects.Push(new Enemy { x = x, y = 10, w = 32, h = 16, alive = true, altSpr = EnAltSpr });
                EnAltSpr = !EnAltSpr;
                x += 16 + 32;
            }
            LastSpawnedX = -32;

            Objects.Push(new Barrier { x = 32, y = l3d.SCREEN_HEIGHT - 64, w = 64, h = 16, alive = true });
            Objects.Push(new Barrier { x = 160, y = l3d.SCREEN_HEIGHT - 64, w = 64, h = 16, alive = true });
            Objects.Push(new Barrier { x = 288, y = l3d.SCREEN_HEIGHT - 64, w = 64, h = 16, alive = true });
            InGame = true;
        }

        static string MakeRelativePath(string filename)
        {
            return l3d.ScriptRes() + "/" + filename;
        }

        static int KeyDown = 0;
        static int keyHeld = 0;
        static bool InGame = false;
        static WavFile shoot, explosion;
        public static void Main()
        {
            shoot = l3d.SoundLoadWave(MakeRelativePath("sh.wav"));
            explosion = l3d.SoundLoadWave(MakeRelativePath("expl.wav"));
            gpu = l3d.GetJsGpu();
            l3d.clearConsole();
            l3d.print("HELLO FROM C# ");
            l3d.print("Now watch this");
            l3d.BottomScreenDisableConsole();
            textures[0] = gpu.load_texture_png(MakeRelativePath("alien.png"));
            textures[1] = gpu.load_texture_png(MakeRelativePath("alien1.png"));
            InitGame();
            while (l3d.aptMainLoop())
            {
                l3d.hidScanInput();
                KeyDown = l3d.hidKeysDown();
                keyHeld = l3d.hidKeysHeld();
                gpu.begin_draw(0, 0);
                if (InGame) GameLoop();
                else MainMenu();
                gpu.draw_on(l3d.GFX_BOTTOM, 0);
                if (InGame)
                    gpu.draw_text_center(1, 0, 1, 1, 0xFFFFFFFF, "Points: " + DefeatedEnemies + "00");
                gpu.draw_text_center(1, 64, 1, 1, 0xFFFFFFFF, "#CSharp");
                gpu.end_draw();
                l3d.gspWaitForVBlank();
                if (Key(KeyDown, KEY.START)) break;
            }
        }

        static void MainMenu()
        {
            gpu.draw_text_center(0, l3d.SCREEN_HEIGHT / 2, 1, 1, 0xFFFFFFFF, "Press A");
            if (Key(KeyDown, KEY.A))
            {
                InitGame();
            }
        }

        public static int EnemyMoveSpeed = 1;
        static int FrameEnemyShoot = 16;
        static int projCoolDown = 16;
        public static bool UseAltSpr = false;
        static int AltSprCounter = 0;
        static void GameLoop()
        {
            if (!player.alive)
            {
                gpu.draw_text_center(0, l3d.SCREEN_HEIGHT / 2, 1, 1, 0xFF0000FF, "You lost");
                if (Key(KeyDown, KEY.A)) InGame = false;
                return;
            }
            EnemyMoveSpeed = 1 + (DefeatedEnemies >> 2);
            if (EnemyMoveSpeed > 5) EnemyMoveSpeed = 5;
            LastSpawnedX += EnemyMoveSpeed;
            ++FrameEnemyShoot;
            ++projCoolDown;
            ++AltSprCounter;
            if (AltSprCounter > 30)
            {
                AltSprCounter = 0;
                UseAltSpr = !UseAltSpr;
            }
            if (LastSpawnedX >= 0)
            {
                LastSpawnedX = -32 - 16;
                Objects.Push(new Enemy { x = LastSpawnedX, y = 10, w = 32, h = 16, alive = true, altSpr = EnAltSpr });
                EnAltSpr = !EnAltSpr;
            }
            for (int i = 0; i < Objects.Length; i++)
            {
                Objects[i].Frame();
                if (!Objects[i].alive) { Objects.Splice(i, 1); --i; continue; }
                Objects[i].Draw();
            }
            if (Key(KeyDown, KEY.B))
            {
                l3d.clearConsole();
                l3d.print(Objects.ToString());
            }
        }
        
        static bool Key(int state, KEY k) { return ((state & (int)k) != 0); }

        static bool Rand(double percent) { return Math.Random() <= percent; }

        static int RandomInt(int max)
        {
            return (int)(Math.Random() * (max + 1));
        }

    }
}
