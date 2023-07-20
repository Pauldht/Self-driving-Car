#ifndef CAR
#define CAR

#include "Point.h"
#include "Circuit.h"

typedef struct{
    
    Point* FL;
    Point* FR;
    Point* BL;
    Point* BR;

    Point *RD;
    Point *RM;
    Point *M;
    Point *LM;
    Point *LD;

    Point* Center;

    float angle;

    float speed;

    int score;
    int sector;

}Car;

Car* newCar();

void freeCar(Car* car);

void InitCar(Car* car, SDL_Surface* img);

void DisplayCar(SDL_Surface* img,SDL_Renderer* renderer,Car* car);

void Rotation(SDL_Renderer* renderer, Car* car, SDL_Surface* img, double angle);

void UpdateCar(Car* car);

void Accelerate(Car* car);
void Brake(Car* car);
void TurnLeft(Car* car);
void TurnRight(Car* car);
void MoveForward(Car* car);

//void FillInput(Circuit* track, Car* car, double input[]);

Point* middle(Point* P, Point* p);

void Input(Car* car, Circuit* track, double input[]);


#endif