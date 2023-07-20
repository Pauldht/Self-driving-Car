#ifndef DISPLAY
#define DISPLAY

#include "Circuit.h"
#include "Car.h"

typedef struct{
    SDL_Window* window;
    SDL_Renderer* renderer;
}Display;

Display* newDisplay();
void freeDisplay(Display* disp);
SDL_Surface* LoadImg(const char* filename);
void displayImage(const Point * p,float angle,SDL_Surface * image,SDL_Renderer * renderer);
SDL_Texture* Create_Texture_From_Image(SDL_Surface* image, SDL_Renderer* renderer);
int Draw_Background(SDL_Renderer* renderer);
void Display_Button(SDL_Renderer* renderer,int mode);
int DrawPoint(SDL_Renderer* renderer, int x, int y);
int DrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
void Draw_Circuit(SDL_Renderer* renderer, Circuit* track);
void DrawCarPoints(SDL_Renderer* renderer,Car* car);
int Intersect_Circuit_Line(Circuit* circuit,Point* P1,Point* P2);
int DetectColision(Car* car, Circuit* circuit);
void DrawCarHitbox(SDL_Renderer* renderer,Car* car);
void DrawZone(SDL_Renderer* renderer,Circuit* track, int zone);
void DrawCarSensor(SDL_Renderer* renderer, Car* car);
void DrawCarSensorF(SDL_Renderer* renderer, Car* car);
void UpdateSensorF(Car* car);
void UpdateSensor(SDL_Renderer* renderer, Car* car);
int DetectColisionSector(Circuit* circuit,int sector,Point* P1,Point* P2);
int Intersect_CheckPoint(Circuit* circuit,Car* car,Point* P1,Point* P2);
void ScoreUpdate(Car* car,Circuit* track);
int CheckCarOrientation(Car* car);

#endif