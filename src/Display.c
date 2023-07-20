#include <SDL2/SDL.h>
#include <SDL2/SDL2_rotozoom.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "Display.h"
#include "Circuit.h"
#include "Car.h"

float PI =  3.141592;


Display* newDisplay()
{
    Display* d = malloc(sizeof(Display));
    assert(d != NULL);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        printf("SDL_Init failed: %s\n", SDL_GetError());
        
    SDL_Window* window = SDL_CreateWindow("CIRCUIT",SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,795,481,SDL_WINDOW_RESIZABLE);

    assert (window != NULL);

    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    assert (renderer != NULL);

    d->window = window;
    d->renderer = renderer;

    return d;
}

void freeDisplay(Display* disp)
{
    SDL_DestroyRenderer(disp->renderer);
    SDL_DestroyWindow(disp->window);
    free(disp);
    SDL_Quit();
}

SDL_Surface* LoadImg(const char* filename)
{
    SDL_Surface* img = NULL;
    img = SDL_LoadBMP(filename);
    assert (img!=NULL);

    return img;
}

SDL_Texture* Create_Texture_From_Image(SDL_Surface* image, SDL_Renderer* renderer)
{
    SDL_Texture* texture = NULL;
    texture = SDL_CreateTextureFromSurface(renderer,image);
    assert (texture!=NULL);
    return texture;
}

int Draw_Background(SDL_Renderer* renderer)
{
    assert(renderer!=NULL);
    if(SDL_SetRenderDrawColor(renderer,21,22,18,255) != 0)
        return EXIT_FAILURE;
    SDL_RenderClear(renderer);
    return EXIT_SUCCESS;
}

void Display_Button(SDL_Renderer* renderer,int mode)
{
    SDL_Surface* b0 = LoadImg("icon/accelerate2.bmp");
    SDL_Surface* b1 = LoadImg("icon/newtrack.bmp");
    SDL_Surface* b2 = LoadImg("icon/reset.bmp");
    SDL_Surface* b3 = NULL;
    SDL_Surface* b4 = LoadImg("icon/close.bmp");

    if(mode == 1)
        b3 = LoadImg("icon/resume.bmp");
    else
        b3 = LoadImg("icon/seebest.bmp");

    SDL_Texture* texture0 = NULL;
    texture0 = SDL_CreateTextureFromSurface(renderer,b0);
    assert (texture0!=NULL);
    SDL_Rect rect0 = {645,42,b0->w,b0->h};
    SDL_RenderCopy(renderer,texture0,NULL,&rect0);
    SDL_DestroyTexture(texture0);

    SDL_Texture* texture = NULL;
    texture = SDL_CreateTextureFromSurface(renderer,b1);
    assert (texture!=NULL);
    SDL_Rect rect = {645,134,b1->w,b1->h};
    SDL_RenderCopy(renderer,texture,NULL,&rect);
    SDL_DestroyTexture(texture);

    SDL_Texture* text = NULL;
    text = SDL_CreateTextureFromSurface(renderer,b2);
    assert (text!=NULL);
    SDL_Rect rect1 = {645,226,b2->w,b2->h};
    SDL_RenderCopy(renderer,text,NULL,&rect1);
    SDL_DestroyTexture(text);

    SDL_Texture* texture2 = NULL;
    texture2 = SDL_CreateTextureFromSurface(renderer,b3);
    assert (texture2!=NULL);
    SDL_Rect rect2 = {645,318,b3->w,b3->h};
    SDL_RenderCopy(renderer,texture2,NULL,&rect2);
    SDL_DestroyTexture(texture2);

    SDL_Texture* texture3 = NULL;
    texture3 = SDL_CreateTextureFromSurface(renderer,b4);
    assert (texture3!=NULL);
    SDL_Rect rect3 = {645,410,b4->w,b4->h};
    SDL_RenderCopy(renderer,texture3,NULL,&rect3);
    SDL_DestroyTexture(texture3);

    SDL_FreeSurface(b0);
    SDL_FreeSurface(b1);
    SDL_FreeSurface(b2);
    SDL_FreeSurface(b3);
    SDL_FreeSurface(b4);
}

int DrawPoint(SDL_Renderer* renderer, int x, int y)
{
    if(SDL_RenderDrawPoint(renderer,x,y)!=0)
        EXIT_FAILURE;
    EXIT_SUCCESS;
    return 0;
}

int DrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2)
{
    if(SDL_RenderDrawLine(renderer,x1,y1,x2,y2)!=0)
        EXIT_FAILURE;
    EXIT_SUCCESS;
    return 0;
}

void Draw_Circuit(SDL_Renderer* renderer, Circuit* track)
{
    int nbPoint = track->nbPoints;

    SDL_SetRenderDrawColor(renderer,239,239,240,255);

    for(int i = 0; i < (nbPoint-1) ; i++)
    {
        DrawLine(renderer,track->in[i]->x,track->in[i]->y,
        track->in[i+1]->x,track->in[i+1]->y);

        DrawLine(renderer,track->out[i]->x,track->out[i]->y,
        track->out[i+1]->x,track->out[i+1]->y);
    }

    SDL_SetRenderDrawColor(renderer,239,239,240,255);

    for(int i = 0; i < (nbPoint-1) ; i++)
    {
        DrawPoint(renderer,track->in[i]->x,track->in[i]->y);

        DrawPoint(renderer,track->out[i]->x,track->out[i]->y);
    }

    SDL_RenderPresent(renderer);
}

void DrawCarPoints(SDL_Renderer* renderer,Car* car)
{
    SDL_SetRenderDrawColor(renderer,243,255,0,255);
    DrawPoint(renderer,(int) car->FL->x,(int) car->FL->y);
    DrawPoint(renderer,(int) car->FR->x,(int) car->FR->y);
    DrawPoint(renderer,(int) car->BL->x,(int) car->BL->y);
    DrawPoint(renderer,(int) car->BR->x,(int) car->BR->y);
    DrawPoint(renderer,(int) car->Center->x, car->Center->y);
}

int Intersect_Circuit_Line(Circuit* circuit,Point* P1,Point* P2)
{
    int NbPoints = circuit->nbPoints;
    Point * p = newPoint(0,0);
    int a = 0;
    int b = 0;

    for(int i = 0; i<(NbPoints-1); i++)
    {
        a = intersectPoint(p,P1,P2,circuit->in[i],circuit->in[i+1]);
        
        if(a!=0)
        {   
            freePoint(p);
            return 1;
        }
    }

    for(int i = 0; i<(NbPoints-1); i++)
    {
        b = intersectPoint(p,P1,P2,circuit->out[i],circuit->out[i+1]);
        
        if(b!=0)
        {
            freePoint(p);
            return 1;
        }
    }

    freePoint(p);

    return 0;
}

int DetectColision(Car* car, Circuit* circuit)
{
    if(Intersect_Circuit_Line(circuit,car->FL,car->FR)==1)
        return 1;
    if(Intersect_Circuit_Line(circuit,car->FR,car->BR)==1)
        return 1;
    if(Intersect_Circuit_Line(circuit,car->BR,car->BL)==1)
        return 1;
    if(Intersect_Circuit_Line(circuit,car->BL,car->FL)==1)
        return 1;
    return 0;
}

void DrawCarHitbox(SDL_Renderer* renderer,Car* car)
{
    SDL_SetRenderDrawColor(renderer,243,255,0,255);
    DrawLine(renderer,car->FL->x,car->FL->y,car->FR->x,car->FR->y);
    DrawLine(renderer,car->FL->x,car->FL->y,car->BL->x,car->BL->y);
    DrawLine(renderer,car->FR->x,car->FR->y,car->BR->x,car->BR->y);
    DrawLine(renderer,car->BL->x,car->BL->y,car->BR->x,car->BR->y);
}

void DrawZone(SDL_Renderer* renderer,Circuit* track, int zone)
{
    SDL_SetRenderDrawColor(renderer,252,93,93,255);
    DrawLine(renderer,track->in[zone]->x,track->in[zone]->y,track->out[zone]->x,track->out[zone]->y);
}

void DrawCarSensorF(SDL_Renderer* renderer, Car* car)
{
    int Sensor_Size = 50;
    Point *RD = newPoint(0,0);
    Point *RM = newPoint(0,0);
    Point *M  = newPoint(0,0);
    Point *LM = newPoint(0,0);
    Point *LD = newPoint(0,0);
    UpdatePoint(RD,car->RD->x,car->RD->y);
    UpdatePoint(RM,car->RM->x,car->RM->y);
    UpdatePoint(M,car->M->x,car->M->y);
    UpdatePoint(LM,car->LM->x,car->LM->y);
    UpdatePoint(LD,car->LD->x,car->LD->y);
    float angle = car->angle;
    float X,Y;
    int signe = 1;

    SDL_SetRenderDrawColor(renderer,145,145,233,255);

    Point * P = middle(car->FR,car->BR);
    X = P->x;
    Y = P->y;

    if((angle >= 0 && angle <= PI) || (angle <= 0 && angle >= -PI) )
    {
        if(angle>=PI/2 || angle <= -PI/2)
            signe = -1;
        UpdatePoint(RD,X + Sensor_Size*cos((PI/2 - angle)*signe), Y + Sensor_Size*sin((PI/2 - angle)*signe)*signe);
        signe = 1;
    }
    else
    {
        if(angle<=-PI/2 || angle >= PI/2)
            signe = -1;
        UpdatePoint(RD,X - Sensor_Size*cos((angle - PI/2))*signe, Y + Sensor_Size*sin((angle- PI/2))*signe);
        signe = 1;
    }

    freePoint(P);

    DrawLine(renderer,RD->x,RD->y,X,Y);

    X = car->FR->x;
    Y = car->FR->y;

    signe = 1;

    if((angle >= 0 && angle <= PI) || (angle <= 0 && angle >= -PI) )
    {
        if(angle>=PI/2 || angle <= -PI/2)
            signe = -1;
        UpdatePoint(RM,X + Sensor_Size*cos((PI/4 - angle)*signe), Y + Sensor_Size*sin((PI/4 - angle)*signe)*signe);
        signe = 1;
    }
    else
    {
        if(angle<=-PI/2 || angle >= PI/2)
            signe = -1;
        UpdatePoint(RM,X - Sensor_Size*cos((angle - PI/4))*signe, Y + Sensor_Size*sin((angle- PI/4))*signe);
        signe = 1;
    }

    DrawLine(renderer,RM->x,RM->y,X,Y);

    Point* P2 = middle(car->FL,car->FR);
    X = P2->x;
    Y = P2->y;

    signe = 1;

    if((angle >= 0 && angle <= PI) || (angle <= 0 && angle >= -PI) )
    {
        if(angle>=PI/2 || angle <= -PI/2)
            signe = -1;
        UpdatePoint(M,X + Sensor_Size*cos((angle)*signe), Y - Sensor_Size*sin((angle)*signe)*signe);
        signe = 1;
    }
    else
    {
        if(angle<=-PI/2 || angle >= PI/2)
            signe = -1;
        UpdatePoint(M,X - Sensor_Size*cos((angle))*signe, Y + Sensor_Size*sin((angle))*signe);
        signe = 1;
    }

    DrawLine(renderer,M->x,M->y,X,Y);

    freePoint(P2);

    X = car->FL->x;
    Y = car->FL->y;

    signe = 1;

    if((angle >= 0 && angle <= PI) || (angle <= 0 && angle >= -PI) )
    {
        if(angle>=PI/2 || angle <= -PI/2)
            signe = -1;
        UpdatePoint(LM,X - Sensor_Size*cos((PI/4 + PI/2 - angle)*signe), Y - Sensor_Size*sin((PI/4 + PI/2 - angle)*signe)*signe);
        signe = 1;
    }
    else
    {
        if(angle<=-PI/2 || angle >= PI/2)
            signe = -1;
        UpdatePoint(LM,X + Sensor_Size*cos((angle - PI/4 - PI/2 ))*signe, Y - Sensor_Size*sin((- PI/4 + angle - PI/2))*signe);
        signe = 1;
    }

    DrawLine(renderer,LM->x,LM->y,X,Y);

    Point* P1 = middle(car->FL,car->BL);
    X = P1->x;
    Y = P1->y;

    signe = 1;

    if((angle >= 0 && angle <= PI) || (angle <= 0 && angle >= -PI) )
    {
        if(angle>=PI/2 || angle <= -PI/2)
            signe = -1;
        UpdatePoint(LD,X + Sensor_Size*cos((PI/2 + PI - angle)*signe), Y + Sensor_Size*sin((PI/2 + PI - angle)*signe)*signe);
        signe = 1;
    }
    else
    {
        if(angle<=-PI/2 || angle >= PI/2)
            signe = -1;
        UpdatePoint(LD,X - Sensor_Size*cos((-PI/2 - PI + angle))*signe, Y + Sensor_Size*sin((-PI/2 - PI + angle))*signe);
        signe = 1;
    }

    DrawLine(renderer,LD->x,LD->y,X,Y);

    UpdatePoint(car->RD,RD->x,RD->y);
    UpdatePoint(car->RM,RM->x,RM->y);
    UpdatePoint(car->M,M->x,M->y);
    UpdatePoint(car->LM,LM->x,LM->y);
    UpdatePoint(car->LD,LD->x,LD->y);

    freePoint(P1);
    free(RD);
    free(RM);
    free(M);
    free(LM);
    free(LD);

    return;
}

int DetectColisionSector(Circuit* circuit,int sector,Point* P1,Point* P2)
{
    int nb = circuit->nbPoints;
    Point* p = newPoint(0,0);

    for(int i = 0; i<nb; i++)
    {
        int a = intersectPoint(p,P1,P2,circuit->in[i],circuit->out[i]);

        if(a != 0 && i >= sector){
            freePoint(p);
            return 1;
        }
    }

    if(sector==0)
    {
        int a = intersectPoint(p,P1,P2,circuit->in[0],circuit->out[0]);

        if(a != 0){
            freePoint(p);
            return 1;
        }
    }

    freePoint(p);

    return 0;
}


int Intersect_CheckPoint(Circuit* circuit,Car* car,Point* P1,Point* P2)
{
    if(DetectColisionSector(circuit,car->sector,P1,P2))
    {
        if(car->sector <= (circuit->nbPoints))
        {
            car->sector++;
        }
        else{
            car->sector = 0;

            if(DetectColisionSector(circuit,car->sector,P1,P2) || DetectColisionSector(circuit,(circuit->nbPoints-1),P1,P2))
            {
                return 1;
            }
        }

        return 1;
    }

    return 0;
}

void ScoreUpdate(Car* car, Circuit* track)
{
    if(Intersect_CheckPoint(track,car,car->FL,car->FR)==1)
        car->score+=1;
    if(Intersect_CheckPoint(track,car,car->FR,car->BR)==1)
        car->score+=1;
    if(Intersect_CheckPoint(track,car,car->BR,car->BL)==1)
        car->score+=1;
    if(Intersect_CheckPoint(track,car,car->BL,car->FL)==1)
        car->score+=1;
}

int CheckCarOrientation(Car* car)
{
    //Définir les Zones 
    // 1 - (0,0)(120,0)(120,120)(0,120)
    // 2 - (120,0)(560,0)(560,180)(120,180)
    // 3 - (560,0)(680,0)(680,120)(560,120)
    // 4 - (560,120)(680,120)(680,260)(560,260)
    // 5 - (560,260)(680,260)(680,380)(560,380)
    // 6 - (120,260)(560,260)(560,380)(120,380)
    // 7 - (0,260)(120,260)(120,380)(0,380)
    // 8 - (0,120)(120,120)(120,260)(0,260)

    SDL_Rect R1,R2,R3,R4,R5,R6,R7,R8;
    R1.x = 0;
    R1.y = 0;
    R1.w = 120;
    R1.h = 120;

    R2.x = 120;
    R2.y = 0;
    R2.w = 440;
    R2.h = 180;

    R3.x = 560;
    R3.y = 0;
    R3.w = 120;
    R3.h = 120;

    R4.x = 560;
    R4.y = 120;
    R4.w = 120;
    R4.h = 140;

    R5.x = 560;
    R5.y = 260;
    R5.w = 120;
    R5.h = 120;

    R6.x = 120;
    R6.y = 260;
    R6.w = 440;
    R6.h = 120;

    R7.x = 0;
    R7.y = 260;
    R7.w = 120;
    R7.h = 120;

    R8.x = 0;
    R8.y = 120;
    R8.w = 120;
    R8.h = 140;

    int a = (int) car->FL->x;
    int b = (int) car->FL->y;
    int A = (int) car->BL->x;
    int B = (int) car->BL->y;


    if(SDL_IntersectRectAndLine(&R1,&a,&b,&A,&B))
    {
        if(car->angle > PI/2 || car->angle < -PI/2)
            return 1;
    }
    if(SDL_IntersectRectAndLine(&R8,&a,&b,&A,&B))
    {
        if(car->angle > -PI || car->angle < -2*PI)
            return 1;
    }
    if(SDL_IntersectRectAndLine(&R7,&a,&b,&A,&B))
    {
        if(car->angle > -PI/2 || car->angle < -7*PI/4)
            return 1;
    }
    if(SDL_IntersectRectAndLine(&R6,&a,&b,&A,&B))
    {
        if(car->angle > -PI/2 || car->angle < -3*PI/2)
            return 1;
    }
    if(SDL_IntersectRectAndLine(&R5,&a,&b,&A,&B))
    {
        if(car->angle > -5*PI/4 || car->angle < -PI/4)
            return 1;
    }
    if(SDL_IntersectRectAndLine(&R4,&a,&b,&A,&B))
    {
        if(car->angle > 0 || car->angle < -PI)
            return 1;
    }
    if(SDL_IntersectRectAndLine(&R3,&a,&b,&A,&B))
    {
        if(car->angle > PI/2 || car->angle < -3*PI/4)
            return 1;
    }
    if(SDL_IntersectRectAndLine(&R2,&a,&b,&A,&B))
    {
        if(car->angle > PI/2 || car->angle < -PI/2)
            return 1;
    }

    return 0;
}