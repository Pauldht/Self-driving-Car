#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Point.h"

Point* newPoint(float x, float y)
{
    Point *p = malloc(sizeof(Point));
    assert (p!=NULL);
    
    p->x = x;
    p->y = y;
    
    return p;
}

void UpdatePoint(Point* p, float x, float y)
{
    p->x = x;
    p->y = y;
}

void freePoint(Point *p)
{
    free(p);
}

float Distance(Point* P1, Point *P2)
{
    return sqrtf(pow(P1->x - P2->x,2) + pow(P1->y - P2->y,2));
}

int intersect (Point * A, Point * B , Point * C , Point * D){

        float Ix, Iy, Jx, Jy;
        float Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
        float m, k;
        Ax = A->x;
        Ay = A->y;
        Bx = B->x;

        By = B->y;
        Cx = C->x;
        Cy = C->y;
        Dx = D->x;
        Dy = D->y;

        Ix = (Ax-Bx);
        Iy = (Ay-By);
        Jx = (Cx-Dx);
        Jy = (Cy-Dy);

        m = (-Ix*Ay + Ix*Cy + Iy*Ax - Iy*Cx)/(Ix*Jy - Iy*Jx);
        k = ( Ax*Jy - Cx*Jy - Jx*Ay + Jx*Cy)/(Ix*Jy - Iy*Jx);
        if((k>=0 && k<=1)&&(m>=0 && m<=1))
            return 1;
        else
            return 0;
}


int intersectPoint(Point* P,Point * A, Point * B , Point * C , Point * D){

        if(intersect(A,B,C,D) == 0)
            return 0;

        float Ix, Iy, Jx, Jy;
        float Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
        float m;

        Ax = A->x;
        Ay = A->y;
        Bx = B->x;
        By = B->y;
        Cx = C->x;
        Cy = C->y;
        Dx = D->x;
        Dy = D->y;

        Ix = (Ax-Bx);
        Iy = (Ay-By);
        Jx = (Cx-Dx);
        Jy = (Cy-Dy);

        m = -(-Ix*Ay + Ix*Cy + Iy*Ax - Iy*Cx)/(Ix*Jy - Iy*Jx);

        P->x =  Cx + m* Jx;
        P->y =  Cy + m* Jy;

        return 1;
}