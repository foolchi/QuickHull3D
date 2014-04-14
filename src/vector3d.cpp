#include "vector3d.h"


Vector3D::Vector3D(){
    x = y = z = 0;
}
Vector3D::Vector3D(const Vector3D &v){
    x = v.x;
    y = v.y;
    z = v.z;
}
Vector3D::Vector3D(double x, double y, double z){
    this->x = x;
    this->y = y;
    this->z = z;
}
double Vector3D::get(int i) const{
    switch(i){
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        return x;
    }
}

void Vector3D::set(int i, double value){
    switch(i){
    case 0:
        x = value; break;
    case 1:
        y = value; break;
    case 2:
        z = value; break;
    default:
        x = value; break;
    }
}

void Vector3D::set(double x, double y, double z){
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector3D::set(Vector3D v1){
    x = v1.x;
    y = v1.y;
    z = v1.z;
}

void Vector3D::set(Vector3D* v1){
    x = v1->x;
    y = v1->y;
    z = v1->z;
}

void Vector3D::add(Vector3D v1, Vector3D v2){
    x = v1.x + v2.x;
    y = v1.y + v2.y;
    z = v1.z + v2.z;
}

void Vector3D::add(Vector3D v1){
    x += v1.x;
    y += v1.y;
    z += v1.z;
}

void Vector3D::sub(Vector3D v1, Vector3D v2){
    x = v1.x - v2.x;
    y = v1.y - v2.y;
    z = v1.z - v2.z;
}

void Vector3D::scale(double s){
    x *= s;
    y *= s;
    z *= s;
}

double Vector3D::norm(){
    return sqrt(x*x + y*y + z*z);
}

double Vector3D::normSquared(){
    return x*x + y*y + z*z;
}



double Vector3D::distance(Vector3D v){
    double dx = x - v.x;
    double dy = y - v.y;
    double dz = z - v.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

double Vector3D::distanceSquared(Vector3D v){
    double dx = x - v.x;
    double dy = y - v.y;
    double dz = z - v.z;
    return dx*dx + dy*dy + dz*dz;
}

double Vector3D::dot(Vector3D v){
    return x*v.x + y*v.y + z*v.z;
}

void Vector3D::normalize(){
    double lenSqr = x*x + y*y + z*z;
    double err = lenSqr - 1;
    if (ABS(err) > 2 * DOUBLEPRECISION){
        lenSqr = sqrt(lenSqr);
        x /= lenSqr;
        y /= lenSqr;
        y /= lenSqr;
    }
}

void Vector3D::setZero(){
    x = y = z = 0;
}

void Vector3D::cross(Vector3D v1, Vector3D v2){
    x = v1.y*v2.z - v1.z*v2.y;
    y = v1.z*v2.x - v1.x*v2.z;
    z = v1.x*v2.y - v1.y*v2.x;
}

void Vector3D::setRandom(double lower, double upper){
    double range = upper - lower;
    srand(time(NULL));
    x = lower + range * rand() / RAND_MAX;
    y = lower + range * rand() / RAND_MAX;
    z = lower + range * rand() / RAND_MAX;
}
