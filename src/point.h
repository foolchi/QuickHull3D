#ifndef POINT_H
#define POINT_H

class Point{
private:
    double *coor;

public:
    Point(){}
    Point (const Point& pnew){
        coor = new double[3];
        coor[0] = pnew.x();
        coor[1] = pnew.y();
        coor[2] = pnew.z();
    }

    Point(double a,double b, double c){ coor = new double[3];coor[0] = a; coor[1] = b; coor[2] = c;}
    Point(double* p){ coor = p;}
    double x() const{ return coor[0];}
    double y() const{ return coor[1];}
    double z() const{ return coor[2];}
    double* getcoor(){
        return coor;
    }
    bool isequal(Point p){
        double *coor2 = p.getcoor();
        for (int i = 0; i < 3; i++)
            if (coor[i] != coor2[i])
                return false;
        return true;
    }
};

#endif // POINT_H
