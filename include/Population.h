#ifndef POPULATION
#define POPULATION

#include "Network.h"

typedef struct{
    Network* Generation;
    int nb_of_population;
    int nb_by_generation;
    float fitness;
}Population;

Population* NewPopulation();
void InitPopulation(Population* P, SDL_Surface* img);
void FreePopulation(Population* P);
void PrintPopulation(Population* P, int specific);
void Set_Network(Network * N, Network *n);
Population* Change_Population(Population* P,SDL_Surface* img);
int BestScore(Population* P);
int BestCarIndex(Population* P);

#endif