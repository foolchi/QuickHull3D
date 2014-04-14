#include "face.h"
using namespace std;
Face::Face(){
    size = 0;
    iscaculated = false;
    vp.clear();
}

void Face::clear(){
    size = 0;
    iscaculated = false;
    vp.clear();
}

void Face::sortbyx(){
    if (size < 2)
        return;
    sort(0);
}

void Face::sort(int axis){
    double currentmax = vp[size-1].getcoor()[axis], currentmin = vp[size-1].getcoor()[axis];
    for (int i = size-1; i > 0; i--)
        for (int j = i-1; j >= 0; j--){
            if (vp[j].getcoor()[axis] > currentmax)
                currentmax = vp[j].getcoor()[axis];
            if (vp[j].getcoor()[axis] < currentmin)
                currentmin = vp[j].getcoor()[axis];
            if (vp[i].getcoor()[axis] > vp[j].getcoor()[axis]){
                Point potemp = vp[i];
                vp[i] = vp[j];
                vp[j] = potemp;
            }
        }
    if (currentmax == currentmin && axis < 2){
        qDebug("Sort again");
        sort(axis+1);
    }
}

void Face::addpoint(Point p){
    if (iscontain(p))
        return;
    vp.push_back(p);
    size++;
}

bool Face::iscontain(Point p){
    // Check whether the point is already in this face.

    for (int i = 0; i < size; i++)
        if (p.isequal(vp[i]))
            return true;
    return false;
}

bool Face::isline(Point p1, Point p2, Point p3){
    // Check whether three points are in the same line

    return ((p3.x()-p1.x())*(p2.y()-p1.y()) == (p3.y()-p1.y())*(p2.x()-p1.x()))
            &&
            ((p3.x()-p1.x())*(p2.z()-p1.z()) == (p3.z()-p1.z())*(p2.x()-p1.x()))
            &&
            ((p3.z()-p1.z())*(p2.y()-p1.y()) == (p3.y()-p1.y())*(p2.z()-p1.z()));
}

bool Face::isface(){
    // Check whether the point vector can form a face

    if (size < 3)
        return false;
    Point p1,p2,p3;
    int i = 3;
    p1 = vp[0]; p2 = vp[1]; p3 = vp[2];

    while (isline(p1,p2,p3)){
        if(i == size)
            return false;
        p3 = vp[i++];
    }
    return true;
}

double* Face::getpara(){
    // Get the parameters of the face
    // (a,b,c,d):
    //      aX + bY + cZ + d = 0

    if (iscaculated)
        return facepara;

    for (int i = 0; i < 4; i++)
        facepara[i] = 0;
    if (size < 3)
        return facepara;
    Point p1,p2,p3;
    int i = 3;
    p1 = vp[0]; p2 = vp[1]; p3 = vp[2];

    while (isline(p1,p2,p3)){
        if(i == size)
            return facepara;
        p3 = vp[i++];
    }

    facepara[0] = (p2.y()-p1.y()) * (p3.z()-p1.z()) - (p2.z()-p1.z()) * (p3.y()-p1.y());
    facepara[1] = (p2.z()-p1.z()) * (p3.x()-p1.x()) - (p2.x()-p1.x()) * (p3.z()-p1.z());
    facepara[2] = (p2.x()-p1.x()) * (p3.y()-p1.y()) - (p2.y()-p1.y()) * (p3.x()-p1.x());
    facepara[3] = -(facepara[0]*p1.x() + facepara[1]*p1.y() + facepara[2]*p1.z());

    // Normalize the parameters
    double paramax = facepara[0];
    for (int i = 1; i < 4; i++)
        if (ABS(facepara[i]) > ABS(paramax))
            paramax = facepara[i];
    if (paramax == 0)
        return facepara;
    for (int i = 0; i < 4; i++)
        facepara[i] /= paramax;
    iscaculated = true;

    return facepara;
}

int Face::relativeposition(Point p){
    // Check the relative position of one point to the face

    double* coor = p.getcoor();
    this->getpara();
    double result = facepara[3];
    for (int i = 0; i < 3; i++)
        result += coor[i]*facepara[i];
    if (result > PRECISION)
        return 1;
    if (result < -PRECISION)
        return -1;
    return 0;
}

bool Face::isequal(Face f){
    // Check if the two faces are identical

    double* para2 = f.getpara();
    this->getpara();
    for (int i = 0; i < 4; i++)
        if (!ISEQUAL(facepara[i],para2[i]))
            return false;
    return true;
}

