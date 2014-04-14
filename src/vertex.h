#ifndef VERTEX_H
#define VERTEX_H
#include "facet.h"
#include "point3d.h"

class Facet;
class Vertex{
public:
    Point3D *pnt;
    int index;
    Vertex *prev, *next;
    Facet *facet;

    Vertex(){
        prev = NULL;
        next = NULL;
        pnt = new Point3D();
    }
    Vertex(double x, double y, double z, int idx){
        prev = NULL;
        next = NULL;
        pnt = new Point3D(x,y,z);
        index = idx;
    }
    ~Vertex(){
        delete[] pnt;
    }
};


#endif // VERTEX_H
