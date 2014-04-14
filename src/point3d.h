#ifndef POINT3D_H
#define POINT3D_H
/**
 * @file point3d.h
 * @brief point in 3D
 */

#include "vector3d.h"

/**
 * @brief Point in 3D
 */
class Point3D: public Vector3D{

public:
    /**
     * @brief Constructor
     */
    Point3D(){}

    /**
     * @brief Constructor
     * @param v another point or vector
     */
    Point3D(Vector3D v){
        x = v.x;
        y = v.y;
        z = v.z;
    }

    /**
     * @brief Constructor
     * @param v pointer of another point or vector
     */
    Point3D(Vector3D* v){
        x = v->x;
        y = v->y;
        z = v->z;
    }

    /**
     * @brief Constructor
     * @param x coordinate x
     * @param y coordinate y
     * @param z coordinate z
     */
    Point3D(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

#endif // POINT3D_H
