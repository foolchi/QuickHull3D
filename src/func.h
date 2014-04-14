#ifndef FUNC_H
#define FUNC_H
#include <QApplication>
#include <QThread>

#define PI 3.14159
#define ABS(x) ((x)<0? (-(x)) : (x))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
#define RAD(x) ((x)*2.0*PI/360.0)
#define ISEQUAL(x,y) (ABS((x)-(y)) < PRECISION)
#define ZOOMSPEED 0.02
#define MAX_EXPONENT 10
#define MAX_FACE 255
#define MIN_SCALE 1
#define MAX_SCALE 10
#define MAX_STEP 100000
#define ROTATE_SPEED 5

const int LABELTEXT_WIDTH = 1;
const int EDITTEXT_WIDTH = 2;
const int ZONO_WIDTH = 8;

const float PRECISION = 1e-6;  //Calculation precision for float
const double DOUBLEPRECISION = 1e-12;  //Calculation precision for double float



#endif // FUNC_H
