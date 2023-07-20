#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "Point.h"
#include "Circuit.h"


Circuit * newCircuit(int nbp)
{
    Circuit * t = malloc(sizeof(Circuit));
    assert(t!=NULL);

    t->in = malloc(nbp * sizeof(Point));
    assert(t->in != NULL);

    t->out = malloc(nbp * sizeof(Point));
    assert(t->out != NULL);

    t->nbPoints = nbp;

    return t;
}

void freeCircuit(Circuit * circuit)
{   
    int nbPoints = circuit->nbPoints;

    for(int i = 0; i<nbPoints; i++)
        free(circuit->in[i]);
    for(int i = 0; i<nbPoints; i++)    
        free(circuit->out[i]);
    free(circuit->in);
    free(circuit->out);
    free(circuit);
}

void SetCircuitIn(Circuit* circuit, Point* p, int index)
{
    circuit->in[index] = p;
}

void SetCircuitOut(Circuit* circuit, Point* p, int index)
{
    circuit->out[index] = p;
}

void PrintCircuit(Circuit* circuit)
{
    int nb = circuit->nbPoints;

    for(int i = 0; i<nb; i++)
        printf("%lf %lf\n",circuit->in[i]->x,circuit->in[i]->y);

    for(int i = 0; i<nb; i++)
        printf("%lf %lf\n",circuit->out[i]->x,circuit->out[i]->y);
}

Circuit* initCircuit(char* filename){

    FILE * f = fopen(filename,"r");
    assert(f != NULL);

    int nbPoint;
    fscanf(f,"%d",&nbPoint);

    Circuit* track = newCircuit(nbPoint);
    assert(track != NULL);

    int x,y;
    for(int i = 0; i<nbPoint ; i++)
    {
        fscanf(f,"%d %d",&x,&y);
        Point* np = newPoint(x,y);
        SetCircuitOut(track,np,i);
    }

    for(int i = 0; i<nbPoint ; i++)
    {
        fscanf(f,"%d %d",&x,&y);
        Point* np = newPoint(x,y);
        SetCircuitIn(track,np,i);
    }

    fclose(f);

    return track;
}

