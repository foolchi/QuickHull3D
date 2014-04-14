#ifndef VECTOR3D_H
#define VECTOR3D_H
#include "func.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

class Vector3D{

public:
    Vector3D();
    Vector3D(const Vector3D &v);
    Vector3D(double x, double y, double z);
    double get(int i) const;
    virtual void set(int i, double value);
    virtual void set(double x, double y, double z);
    virtual void set(Vector3D v1);
    virtual void set(Vector3D* v1);
    virtual void add(Vector3D v1, Vector3D v2);
    virtual void add(Vector3D v1);
    void sub(Vector3D v1, Vector3D v2);
    void scale(double s);
    double norm();
    double normSquared();
    double distance(Vector3D v);
    double distanceSquared(Vector3D v);
    double dot(Vector3D v);
    void normalize();
    void setZero();
    void cross(Vector3D v1, Vector3D v2);
    void setRandom(double lower, double upper);

    double x,y,z;
};


#endif // VECTOR3D_H
