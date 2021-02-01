//inspired by video https://www.youtube.com/watch?v=xW8skO7MFYw
//algorithms from John Carmak wolfenstein 3d
#include <iostream>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <vector>
#include <string>
#include <chrono>
#include <math.h>

int scrWidth = 120; //Lines
int scrHeight = 40; //Columns

float PlayerX = 8.0f; //StartCoordinates
float PlayerY = 8.0f;

float PlayerAngle = 0.0f;

int MapHeight = 16;
int MapWidth = 16;

float FOV = 3.14159/4.0f;

float Depth = 16.0f;

float Speed = 100.0f;

auto tp1 = std::chrono::steady_clock::now();
auto  tp2 = std::chrono::steady_clock::now();

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, ""); //Unicode support
  initscr();
  noecho();
  nodelay(stdscr,TRUE);

  /// TODO: separate class, with serialization
  std::wstring Map;    //"." for free, "#" for wall
  Map += L"################";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"#..............#";
  Map += L"################";

  wchar_t *screen = new wchar_t[scrWidth*scrHeight]; //all screen frame symbols
  for (int c=0; c<scrHeight*scrWidth; c++) {
    screen[c] = ' ';
  }

  while(1){
    tp2 = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = tp2 - tp1;
    tp1 = tp2;
    float ElapsedTime = elapsedTime.count();

    //Handle input, notice, getch non blocking accordiing to ncurses options
    int userInput = getch();

    //Handle "WASD"
    if (userInput == 'A' || userInput == 'a'){
      PlayerAngle -= (Speed * 0.75f) * ElapsedTime;}

    if (userInput == 'd' || userInput=='D')
      PlayerAngle += (Speed * 0.75f) * ElapsedTime;

    if (userInput=='w' || userInput== 'W' )
      {
        PlayerX += sinf(PlayerAngle) * Speed * ElapsedTime;
        PlayerY += cosf(PlayerAngle) * Speed * ElapsedTime;
        if (Map.c_str()[(int)PlayerX * MapWidth + (int)PlayerY] == '#' || PlayerX>=float(MapWidth) || PlayerY>=float(MapHeight))
          {
            PlayerX -= sinf(PlayerAngle) * Speed * ElapsedTime;
            PlayerY -= cosf(PlayerAngle) * Speed * ElapsedTime;
          }
      }

    if (userInput=='s' || userInput=='S' )
      {
        PlayerX -= sinf(PlayerAngle) * Speed * ElapsedTime;
        PlayerY -= cosf(PlayerAngle) * Speed * ElapsedTime;
        if (Map.c_str()[(int)PlayerX * MapWidth + (int)PlayerY] == '#'  || PlayerX>=MapWidth || PlayerY>=MapHeight)
          {
            PlayerX += sinf(PlayerAngle) * Speed * ElapsedTime;
            PlayerY += cosf(PlayerAngle) * Speed * ElapsedTime;
          }
      }

    //iterate throgh columns
    for (int x = 0; x < scrWidth; x++) {
      float RayAngle = (PlayerAngle - FOV/2.0f) + ((float)x / (float)scrWidth) * FOV;

      float StepSize = 0.1f;
      float DistanceToObject = 0.0f;

      bool isHitWall = false;
      bool isBoundary = false;

      float EyeX = sinf(RayAngle); //Units vectors (just direction of ray)
      float EyeY = cosf(RayAngle);
      // Ray tracing, send ray, go in ray diretion by step size until hit object or get out of bounds
      while (!isHitWall && DistanceToObject < Depth)
        {
          DistanceToObject += StepSize;
          int TestX = (int)(PlayerX + EyeX * DistanceToObject);
          int TestY = (int)(PlayerY + EyeY * DistanceToObject);
          // Test if ray is out of bounds
          if (TestX < 0 || TestX >= MapWidth || TestY < 0 || TestY >= MapHeight)
            {
              isHitWall = true;			// Just set distance to maximum depth
              DistanceToObject = Depth;
            }
          else
            {
              if (Map.c_str()[TestX * MapWidth + TestY] == '#'){
                // Ray hit wall
                isHitWall = true;
              }
            }

        }
          int Ceiling = (float)(scrHeight/2.0) - scrHeight / ((float)DistanceToObject);
          int Floor = scrHeight - Ceiling;

          short nShade = ' ' ; //Wall texture depends on perspectivea
          if (DistanceToObject <= Depth / 4.0f)			nShade = 0x2588;	// Very close
          else if (DistanceToObject < Depth / 3.0f)		nShade = 0x2593;
          else if (DistanceToObject < Depth / 2.0f)		nShade = 0x2592;
          else if (DistanceToObject < Depth)			nShade = 0x2591;
          else{
            nShade = ' ';		// Too far away
          }

          if (isBoundary){
            nShade = ' '; }// Black it out
          for (int y = 0; y < scrHeight; y++)
            {
              // Each Row
              if(y <= Ceiling) {//Ceil
                screen[y*scrWidth + x] = nShade;
             }
              else if(y > Ceiling && y <= Floor) //Wall
                screen[y*scrWidth + x] = nShade;
              else // Floor
                {
                  // Shade floor based on distance
                  float b = 1.0f - (((float)y -scrHeight/2.0f) / ((float)scrHeight / 2.0f));
                  if (b < 0.25)		nShade = '#';
                  else if (b < 0.5)	nShade = 'x';
                  else if (b < 0.75)	nShade = '.';
                  else if (b < 0.9)	nShade = '-';
                  else				nShade = ' ';
                  screen[y*scrWidth + x] = nShade;
                }
            }
        }

      erase();

      for (int x = 0; x < MapWidth; x++){
        for (int y = 0; y < MapWidth; y++)
          {
            screen[(y+1)*scrWidth + x] = Map[y * MapWidth + x];
          }
      }
      screen[(int)(PlayerX+1) * scrWidth + (int)PlayerY] = 'P';

      addwstr(screen);
      //Stats render
      std::string kek  = "PlayerX=" + std::to_string(PlayerX)+" PlayerY="+std::to_string(PlayerY) + " PlayerAngle=" + std::to_string(PlayerAngle); //+ " debugCounter="+ std::to_string(counterTooFar) + "RayAngle="+std::to_string(RayAngle)+ " EyeX "+std::to_string(EyeX_)+ " EyeY "+std::to_string(EyeY_);
      mvprintw(scrHeight-1,3, "%s", kek.c_str());
      std::string FPS = "FPS="+ std::to_string(1.0f/ElapsedTime);
      mvprintw(1,3, FPS.c_str());
      refresh();
  }

    getch();
    endwin();
    return 0;
  }
