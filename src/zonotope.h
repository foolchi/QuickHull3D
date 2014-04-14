#ifndef ZONOTOPE_H
#define ZONOTOPE_H

/**
 * @file zonotope.h
 * @brief class for calculate
 */

#include <QtGlobal>
#include <QString>
#include <QChar>
#include "face.h"
#include <vector>
#include "quickhull3d.h"

using namespace std;

/**
 * @brief The Zonotope class
 */
class Zonotope
{

public:
    /**
     * @brief Constructor
     */
    Zonotope(){
        x = y = z = "";
    }

    /**
     * @brief Constructor
     * @param x String x
     * @param y String y
     * @param z String z
     */
    Zonotope(QString x, QString y, QString z);

    /**
     * @brief Constructor
     * @param zp another zonotope
     */
    Zonotope(const Zonotope &zp){
        x = zp.getStringX();
        y = zp.getStringY();
        z = zp.getStringZ();
        tmax = zp.getmaxtime();
        tstep = zp.gettimestep();
        dynamic = zp.isdynamic();
        transform_dynamic(x, y, z);
    }

    /**
     * @brief Constructor
     * @param x String x
     * @param y String y
     * @param z String z
     * @param Dynamic dynamic(true) or static(false) mode
     */
    Zonotope(QString x, QString y, QString z, bool Dynamic);

    QString getStringX() const{
        return x;
    }
    QString getStringY() const{
        return y;
    }
    QString getStringZ() const{
        return z;
    }
    bool isdynamic() const{
        return dynamic;
    }

    /**
     * @brief Update the coefficient (dynamic mode)
     * @param t time
     */
    void coeff_update(double t);

    /**
     * @brief Get number of points
     * @return number of points, 2^(numberofvariable)
     */
    int getnumberofpoints();

    /**
     * @brief Get number of variable
     * @return number of variable
     */
    int getnumberofvariable();

    /**
     * @brief Get the coefficients
     * @return the coefficients
     */
    virtual double** getcoeff();

    /**
     * @brief Get the coefficients at a given time (dynamic mode)
     * @param time time
     * @return coefficients of a given time
     */
    virtual double** getcoeff(double time);

    /**
     * @brief Get all the points
     * @return all the points
     */
    double** getpoints();

    /**
     * @brief Get all the faces
     * @return all the faces
     */
    vector<Face> getAllFace();

    /**
     * @brief Set max time (dynamic mode)
     * @param tmax max time
     */
    void setmaxtime(double tmax){
        if (tmax > 0)
            this->tmax = tmax;
        else{
            dynamic = false;
            this->tmax = 0;
        }
    }

    /**
     * @brief Set time step (dynamic mode)
     * @param tstep time step
     */
    void settimestep(double tstep){
        if (!dynamic)
            return;
        if (tstep >= tmax){
            dynamic = false;
            this->tstep = 0;
        }
        else
            this->tstep = tstep;
    }
    double getmaxtime() const{
        return tmax;
    }

    double gettimestep() const{
        return tstep;
    }

public slots:
    bool verification(QString x, QString y, QString z);

private:

    QString x, y, z; /*!< String x, y, z */

    /**
     * @brief Transform the strings
     * @param x String x
     * @param y String y
     * @param z String z
     */
    void transform(QString x, QString y, QString z);

    /**
     * @brief Transform the strings (dynamic mode)
     * @param x String x
     * @param y String y
     * @param z String z
     */
    void transform_dynamic(QString x, QString y, QString z);

    /**
     * @brief Transform one string
     * @param x String to be transformed
     * @return coefficients of the string
     */
    double* trans(QString x);// Transform each equation

    /**
     * @brief Transform one string (dynamic mode)
     * @param x String to be transformed
     * @return coefficients of the string
     */
    double **trans_dynamic(QString x);

    /**
     * @brief vector multiplication
     * @param a vector a
     * @param b vector b
     * @param d length
     * @return vector multiplication result
     */
    double multi(double* a, double* b, int d);// Vector multiplication

    double** coeff; /*!< Coefficients */
    double*** coeff_dynamic; /*!< Dynamic coefficients */
    int numberofvariable;
    int numberofdynamicvariable;
    int numberoftotalvariable;
    double** coor;
    double tmax, tstep;
    bool dynamic;
};

/**
 * @brief Generate faces of the convex from points (brutal force)
 * @param points points
 * @param n number of points
 * @return vector of faces that can form a convex
 */
vector<Face> generateface(double** points, int n);

/**
 * @brief Generate faces of the convex from points (brutal force)
 * @param points points
 * @param n number of points
 * @return vector of faces that can form a convex
 */
vector<Face> generateface2(double** points, int n);

/**
 * @brief Print all the faces
 * @param vp vector of faces
 */
void printface(vector<Face> vp);

/**
 * @brief normalize the points
 * @param points
 * @param n number of points
 * @return points normalized
 */
double** normalize(double** points, int n);

class CheckPoint{
public:
    CheckPoint(double** points, int n){
        this->points = points;
        this->n = n;
    }

    int getnum(){
        return n;
    }
    double** getpoints(){
        return points;
    }

    void check(){

    }

private:

    double** points;
    int n;
};

#endif // ZONOTOPE_H
