#ifndef FACE_H
#define FACE_H
/**
 * @file face.h
 * @brief Face class for display
 *
 */

#include <vector>
#include "point.h"
#include <math.h>
#include "func.h"
#include <QApplication>
using namespace std;

/**
 * @brief The Face class
 *
 * Create face for display from points
 */
class Face
{

private:
    vector<Point> vp; /*< Points set */
    int size; /*< Number of points */
    double facepara[4]; /*< Vector orthogonal and offset */
    bool iscaculated; /*< True if the parameters of the face have been calculated */

public:

    /**
     * @brief Constructor
     */
    Face();

    /**
     * @brief Delete all the points from the face
     */
    void clear();

    /**
     * @brief Get the number of points
     * @return The number of points
     */
    int getsize(){ return size;}

    /**
     * @brief Get the points set
     * @return A vector containing all the points
     */
    vector<Point> getvp(){ return vp;}

    /**
     * @brief Sort all the points by x axis
     *
     * Sort all the points by x axis, if all the points have the same x coordiante,
     * sort again by y, and if all the points have the same y coordiante,
     * sort agian by z
     */
    void sortbyx();

    /**
     * @brief Sort all the points by a given axis
     * @param Axis: 0:x, 1:y, 2:z
     */
    void sort(int axis);

    /**
     * @brief Add a point to the face
     * @param Point to be added
     */
    void addpoint(Point p);

    /**
     * @brief Check whether the point is already in this face
     * @param Point to be checked
     * @return True if contained, false if not contained
     */
    bool iscontain(Point p);

    /**
     * @brief Check whether three points are in the same line
     * @param Point 1
     * @param Point 2
     * @param Point 3
     * @return True if they are in the same line, false if not
     */
    bool isline(Point p1, Point p2, Point p3);

    /**
     * @brief Check whether the points can form a face
     * @return True if can, false if can not
     */
    bool isface();

    /**
     * @brief Get the parameters of the face
     * @return Four parameters (a,b,c,d) : aX + bY + cZ +d = 0
     */
    double* getpara();

    /**
     * @brief Check the relative position of one point to the face
     * @param Point
     * @return Calculate (aX + bY + cZ + d), 1 if positive, -1 if negative, 0 if this point is on the face
     */
    int relativeposition(Point p);

    /**
     * @brief Check if the two faces are identical
     * @param Another face
     * @return True if identical, false if not
     */
    bool isequal(Face f);

};

#endif // FACE_H
