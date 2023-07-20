#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_rotozoom.h>
#include <math.h>
#include "Point.h"
#include "Car.h"
#include "Display.h"
#include "Circuit.h"

float _Pi =  3.141592;

Car* newCar()
{
        Car* newCar = malloc(sizeof(Car));
        assert(newCar!=NULL);

        newCar->FL = newPoint(0,0);
        newCar->FR = newPoint(0,0);
        newCar->BL = newPoint(0,0);
        newCar->BR = newPoint(0,0);

        newCar->Center = newPoint(0,0);

        newCar->RD = newPoint(0,0);
        newCar->RM = newPoint(0,0);
        newCar->M = newPoint(0,0);
        newCar->LM = newPoint(0,0);
        newCar->LD = newPoint(0,0);

        newCar->angle = 0.0;

        newCar->speed = 0.0;

        newCar->score = 0;
        newCar->sector = 0;

        return newCar;
}

void freeCar(Car* car)
{
    freePoint(car->FL);
    freePoint(car->FR);
    freePoint(car->BL);
    freePoint(car->BR);
    freePoint(car->Center);

    freePoint(car->RD);
    freePoint(car->RM);
    freePoint(car->M);
    freePoint(car->LM);
    freePoint(car->LD);

    free(car);
}

void InitCar(Car* car, SDL_Surface* img)
{
    int height = img->h;
    int width = img->w;

    UpdatePoint(car->FL,230,20);
    UpdatePoint(car->FR,230,20+height);
    UpdatePoint(car->BL,230-width,20);
    UpdatePoint(car->BR,230-width,20+height);
    UpdatePoint(car->Center,(car->BL->x+car->FL->x)/2,(car->BL->y+car->BR->y)/2);
}

void UpdateCar(Car* car)
{
    float x,y;

    x = car->FL->x;
    y = car->FL->y;

    UpdatePoint(car->FL,x,y);

    x = car->FR->x;
    y = car->FR->y;

    UpdatePoint(car->FR,x,y);

    x = car->BL->x;
    y = car->BL->y;

    UpdatePoint(car->BL,x,y);

    x = car->BR->x;
    y = car->BR->y;

    UpdatePoint(car->BR,x,y);
}

void DisplayCar(SDL_Surface* img,SDL_Renderer*renderer,Car* car)
{
    SDL_Texture* texture = NULL;
    texture = SDL_CreateTextureFromSurface(renderer,img);
    assert (texture!=NULL);
    SDL_Rect rect = {car->Center->x - img->w/2,car->Center->y - img->h/2,img->w,img->h};
    SDL_RenderCopy(renderer,texture,NULL,&rect);
    SDL_DestroyTexture(texture);
    DrawCarSensorF(renderer,car);
}

void Rotation(SDL_Renderer* renderer, Car* car, SDL_Surface* img, double angle)
{
    SDL_Surface* image = rotozoomSurface(img,angle,1.0,1);
    assert(image!=NULL);
    DisplayCar(image,renderer,car);
    SDL_FreeSurface(image);
}

void Accelerate(Car* car)
{
    if (car->speed < 1)
		car->speed += 0.25;
	else if (car->speed < 2.5)
		car->speed += 0.5;
	else if (car->speed < 5)
		car->speed += 0.2;
	
	if(car->speed > 5)
		car->speed = 5;
}

void Brake(Car* car)
{
    if(car->speed > 0.25)
		car->speed -= 0.2;
	if(car->speed > 0.1)
		car->speed -= 0.1;
}

void TurnLeft(Car* car)
{
    double angle = (_Pi/128);
    double coss = cos(angle);
    double sinn = sin(angle);
    float xM,yM,xx,yy, x, y;
    
    car->angle += angle;
    if(car->angle==2*_Pi)
        car->angle=0;

    x = car->FL->x;
    y = car->FL->y;

    xM = x - car->Center->x;
    yM = y - car->Center->y;

    xx = xM*coss + yM*sinn + car->Center->x;
    yy = -xM*sinn + yM*coss + car->Center->y;

    UpdatePoint(car->FL,xx,yy);

    x = car->FR->x;
    y = car->FR->y;

    xM = x - car->Center->x;
    yM = y - car->Center->y;

    xx = xM*coss + yM*sinn + car->Center->x;
    yy = -xM*sinn + yM*coss + car->Center->y;

    UpdatePoint(car->FR,xx,yy);

    x = car->BL->x;
    y = car->BL->y;

    xM = x - car->Center->x;
    yM = y - car->Center->y;

    xx = xM*coss + yM*sinn + car->Center->x;
    yy = -xM*sinn + yM*coss + car->Center->y;

    UpdatePoint(car->BL,xx,yy);

    x = car->BR->x;
    y = car->BR->y;

    xM = x - car->Center->x;
    yM = y - car->Center->y;

    xx = xM*coss + yM*sinn + car->Center->x;
    yy = -xM*sinn + yM*coss + car->Center->y;

    UpdatePoint(car->BR,xx,yy);
}

void TurnRight(Car* car)
{
    double angle = _Pi/128;
    double coss = cos(angle);
    double sinn = sin(angle);
    float xM,yM,xx,yy, x, y;

    car->angle -= angle;
    if(car->angle==-2*_Pi)
        car->angle=0;

    x = car->FL->x;
    y = car->FL->y;

    xM = x - car->Center->x;
    yM = y - car->Center->y;

    xx = xM*coss - yM*sinn + car->Center->x;
    yy = xM*sinn + yM*coss + car->Center->y;

    UpdatePoint(car->FL,xx,yy);

    x = car->FR->x;
    y = car->FR->y;

    xM = x - car->Center->x;
    yM = y - car->Center->y;

    xx = xM*coss - yM*sinn + car->Center->x;
    yy = xM*sinn + yM*coss + car->Center->y;

    UpdatePoint(car->FR,xx,yy);

    x = car->BL->x;
    y = car->BL->y;

    xM = x - car->Center->x;
    yM = y - car->Center->y;

    xx = xM*coss - yM*sinn + car->Center->x;
    yy = xM*sinn + yM*coss + car->Center->y;

    UpdatePoint(car->BL,xx,yy);

    x = car->BR->x;
    y = car->BR->y;

    xM = x - car->Center->x;
    yM = y - car->Center->y;

    xx = xM*coss - yM*sinn + car->Center->x;
    yy = xM*sinn + yM*coss + car->Center->y;

    UpdatePoint(car->BR,xx,yy);
}

void MoveForward(Car* car)
{
    float speed = car->speed;
    float angle = car->angle;

    float Xshift = cos(angle)*speed;
    float Yshift = sin(angle)*speed*-1;

    float x,y,xM,yM;

    x = car->FL->x;
    y = car->FL->y;

    xM = x + Xshift;
    yM = y + Yshift;

    UpdatePoint(car->FL,xM,yM);

    x = car->FR->x;
    y = car->FR->y;

    UpdatePoint(car->FR,x+Xshift,y+Yshift);

    x = car->BL->x;
    y = car->BL->y;

    UpdatePoint(car->BL,x+Xshift,y+Yshift);

    x = car->BR->x;
    y = car->BR->y;

    UpdatePoint(car->BR,x+Xshift,y+Yshift);

    x = car->Center->x;
    y = car->Center->y;

    UpdatePoint(car->Center,x+Xshift,y+Yshift);
}

Point* middle(Point* P, Point* p)
{
    double X,Y,XX,YY;

    X = P->x;
    Y = P->y;

    XX = p->x;
    YY = p->y;

    Point* MID = newPoint(0,0);

    MID->x = (X + XX)/2;
    MID->y = (Y + YY)/2;

    return MID;
}


void Input(Car* car, Circuit* track, double input[])
{
    Point *B1 = newPoint(0,0);
    Point *B2 = newPoint(0,0);
    Point *B3 = newPoint(0,0);
    Point *B4 = newPoint(0,0);
    Point *B5 = newPoint(0,0);

    B1->x = (car->FL->x + car->BL->x)/2;
    B1->y = (car->FL->y + car->BL->y)/2;
    UpdatePoint(B2,car->FL->x,car->FL->y);
    B3->x = (car->FL->x + car->FR->x)/2;
    B3->y = (car->FL->y + car->FR->y)/2;
    UpdatePoint(B4,car->FR->x,car->FR->y);
    B5->x = (car->FR->x + car->BR->x)/2;
    B5->y = (car->FR->y + car->BR->y)/2;

    Point* P = newPoint(0,0);

    for(int i = 0; i<6; i++)
        input[i] = 1;

    for(int y =(track->nbPoints-1); y>0; y--)
    {
        if(intersectPoint(P,B1,car->LD,track->out[y],track->out[y-1])!=0)
            input[0] = (Distance(P, B1));
        if(intersectPoint(P,B1,car->LD,track->in[y],track->in[y-1])!=0)        
            input[0] = Distance(P, B1);
    }

    for(int y =(track->nbPoints-1); y>0; y--)
    {
        if(intersectPoint(P,B2,car->LM,track->out[y],track->out[y-1])!=0)
            input[1] = Distance(P, B2);
        if(intersectPoint(P,B2,car->LM,track->in[y],track->in[y-1])!=0)        
            input[1] = Distance(P, B2);
    }

    for(int y =(track->nbPoints-1); y>0; y--)
    {
        if(intersectPoint(P,B3,car->M,track->out[y],track->out[y-1])!=0)
            input[2] = Distance(P, B3);
        if(intersectPoint(P,B3,car->M,track->in[y],track->in[y-1])!=0)       
            input[2] = Distance(P, B3);
    }

   for(int y =(track->nbPoints-1); y>0; y--)
    {
        if(intersectPoint(P,B4,car->RM,track->out[y],track->out[y-1])!=0)
            input[3] = Distance(P, B4);
        if(intersectPoint(P,B4,car->RM,track->in[y],track->in[y-1])!=0)        
            input[3] = Distance(P, B4);
    }

    for(int y =(track->nbPoints-1); y>0; y--)
    {
        if(intersectPoint(P,B5,car->RD,track->out[y],track->out[y-1])!=0)
            input[4] = Distance(P, B5);
        if(intersectPoint(P,B5,car->RD,track->in[y],track->in[y-1])!=0)        
            input[4] = Distance(P, B5);
    }

    input[5] = car->speed/5;

    for(int i = 0; i<5; i++)
    {
        if(input[i]!=1)
            input[i]/=50;
    }

    freePoint(P);
    freePoint(B1);
    freePoint(B2);
    freePoint(B3);
    freePoint(B4);
    freePoint(B5);   
}
