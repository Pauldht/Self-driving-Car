#ifndef CIRCUIT
#define CIRCUIT

#include "Point.h"

typedef struct{
    Point ** in;
    Point ** out;
    int nbPoints;
}Circuit;

Circuit * newCircuit(int nbp);
void freeCircuit(Circuit * circuit);
void SetCircuitIn(Circuit* circuit, Point* p, int index);
void SetCircuitOut(Circuit* circuit, Point* p, int index);
void PrintCircuit(Circuit* circuit);
Circuit* initCircuit(char* filename);

#endif