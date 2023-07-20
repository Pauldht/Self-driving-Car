#ifndef SIMULATION
#define SIMULATION 

#include <SDL2/SDL.h>
#include "Display.h"
#include "Circuit.h"
#include "Population.h"

typedef struct{
    Display* disp;
    Circuit* track;
    SDL_Surface* img;
    Population* P;
}Simulation;

Simulation* NewSimulation();

void FreeSimulation(Simulation* sim);

void Display_Info(Simulation* sim, int nb_g, int fitness);

void delay(int * lastTime, int frameRate);

void Start_process();

#endif