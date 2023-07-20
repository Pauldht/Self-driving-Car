#ifndef POINT
#define POINT


typedef struct{
    float x;
    float y;
}Point;

Point * newPoint(float x, float y);

void UpdatePoint(Point* p, float x, float y);

void freePoint(Point *p);

Point* Intersection(Point* P1, Point* P2, Point* P3, Point* P4);

float Distance(Point* P1, Point *P2);

int intersect (Point * A, Point * B , Point * C , Point * D);

int intersectPoint(Point* P,Point * A, Point * B , Point * C , Point * D);

#endif