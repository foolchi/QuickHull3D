#ifndef ZONOVIEW_H
#define ZONOVIEW_H

/**
 * @file zonoview.h
 * @brief Class to show zonotope
 */


#include <QtOpenGL/QGLFormat>
#include <QMainWindow>
#include <vector>
#include "face.h"
#include "zonotope.h"
#include "calcthread.h"
#include <QThread>
#include <QLayout>
#include <QDialog>
#include <MyQSlider.h>
#include <QFormLayout>
#include <QLineEdit>
#include <QDir>
#include <QTime>
using namespace std;

class ZonoView : public QGLWidget
{
    Q_OBJECT
public:

    /**
     * @brief Constructor
     * @param Pointer of parent widget, empty if no parent widget
     */
    explicit ZonoView(QWidget *parent = 0);

    /**
     * @brief Set the default equations for zonoview
     * @param ex : Equation x
     * @param ey : Equation y
     * @param ez : Equation z
     */
    void setDefaultEquation(QString ex, QString ey, QString ez);

    /**
     * @brief Set the faces of a zonotope to show
     * @param Vector of faces
     */
    void setface(vector<Face> vfin);

public slots:

    /**
     * @brief Stop rotate
     */
    void rotatestop(){
        rotate = false;
    }

    /**
     * @brief Right rotate
     */
    void rotateright(){
        rotate = true;

        while (rotate){
        qDebug("Rotate right");
        if (ABS(yrot) > 90 && ABS(yrot) < 180)
            xrot += ROTATE_SPEED;
        else
            xrot -= ROTATE_SPEED;

        while (xrot < -180) xrot += 360;
        while (xrot > 180) xrot -= 360;

        updateGL();
        QApplication::processEvents();
        }
    }

    /**
     * @brief Left rotate
     */
    void rotateleft(){
        rotate = true;

        while (rotate){
        if (ABS(yrot) > 90 && ABS(yrot) < 180)
            xrot -= ROTATE_SPEED;
        else
            xrot += ROTATE_SPEED;

        while (xrot < -180) xrot += 360;
        while (xrot > 180) xrot -= 360;

        updateGL();
        QApplication::processEvents();
        }

    }

    /**
     * @brief Up rotate
     */
    void rotateup(){
        rotate = true;

        while (rotate){
        yrot += ROTATE_SPEED;
        if (yrot > 180) yrot -= 360;
        if (yrot < -180) yrot += 360;
        updateGL();
        QApplication::processEvents();
        }
    }

    /**
     * @brief Down rotate
     */
    void rotatedown(){
        rotate = true;
        while (rotate){
        yrot -= ROTATE_SPEED;
        if (yrot > 180) yrot -= 360;
        if (yrot < -180) yrot += 360;
        updateGL();
        QApplication::processEvents();
        }

    }

    /**
     * @brief Face changed
     * @param vf pointer of vector face to be changed
     */
    void faceChanged(int * vf){
        setface(*((vector<Face> *)vf));
        initializeGL();
        show();
        updateGL();
        emit updated();
    }

    /**
     * @brief Change equation x
     * @param Equation x
     */
    void setX(QString ex){
        this->ex = ex;
        equationchanged = true;
    }

    /**
     * @brief Change equation y
     * @param Equation y
     */
    void setY(QString ey){
        this->ey = ey;
        equationchanged = true;
    }

    /**
     * @brief Change equation z
     * @param Equation z
     */
    void setZ(QString ez){
        this->ez = ez;
        equationchanged = true;
    }

    /**
     * @brief Set zoom scale
     * @param scale
     */
    void setScale(int scale){
        if (scale > 0){
            xscale = yscale = zscale = 1 + 1.0 * scale * MAX_SCALE / 100;
        }
        else
            xscale = yscale = zscale = 1 + 1.0 * scale * MIN_SCALE / 100;
        updateGL();
    }

    /**
     * @brief Save the zonoview automatic
     */
    void savePicture(){
        QTime currenttime = QTime::currentTime();

        QString filename = QDir::current().absolutePath().append("/").append(currenttime.toString(Qt::TextDate)).append(".jpg");
        QImage zonotopeImage = this->grabFrameBuffer();
        qDebug("Try Saving");
        if(!(zonotopeImage.save(filename,0,100)))
            qDebug("Error Saving");
        else
            qDebug("Saved");
    }

    /**
     * @brief Show the zonotope
     */
    void startStatic();

    /**
     * @brief Show the zonotope (dynamic mode)
     */
    void startDynamic();

    /**
     * @brief Set max time
     * @param t_max max time
     */
    void setTmax(QString t_max){
        bool legal = true;
        if (t_max.isEmpty()){
            this->t_max = 1;
            return;
        }
        this->t_max = t_max.toDouble(&legal);
        if (!legal)
            qDebug("Wrong input");
    }

    /**
     * @brief Set step time
     * @param t_step step time
     */
    void setTstep(QString t_step){
        bool legal = true;
        if (t_step.isEmpty()){
            this->t_step = 1;
            return;
        }
        this->t_step = t_step.toDouble(&legal);
        if (!legal)
            qDebug("Wrong input");
    }

    /**
     * @brief setTransparent
     * @param transparent
     */
    void setTransparent(int transparent){
        this->transparent = transparent;
    }

signals:

    /**
     * @brief Face updated
     */
    void updated();

    /**
     * @brief Scale changed
     * @param scale
     */
    void scaleChanged(int scale);

protected:

    /**
     * @brief Initialization for zonoview
     */
    void initializeGL();

    /**
     * @brief Adjust the size
     * @param width : Widget width
     * @param height : Widget height
     */
    void resizeGL(int width, int height);

    /**
     * @brief Draw zonotope
     */
    void paintGL();

    /**
     * @brief Define action when mouse pressed,
     * right button press for rotation
     * @param Mouse event
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief Define action when mouse moved,
     * when the right button is pressed,
     * movement of mouse will cause rotation
     * @param Mouse event
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief Define mouse release event,
     * clear the mouse press status
     * @param Mouse event
     */
    void mouseReleaseEvent(QMouseEvent *event);

    /**
     * @brief Define mouse wheel action,
     * to zoom the graph
     * @param Mouse wheel event
     */
    void wheelEvent(QWheelEvent *event);

private:

    /**
     * @brief Draw zonotope
     */
    void draw();

    /**
     * @brief Initialization of the parameters
     */
    void initView();

    void initColor(int numberofface);

    QString ex, ey, ez; /*!< Equations for x, y and z */
    bool isinit; /*!< Check if the parameters are initialzed */
    QColor* faceColors; /*!< Define the colors for each face */
    QPoint lastPos; /*!< Storage the last position of the mouse */
    vector<Face> vf; /*!< All faces of zonotope */

    //bool rotate;

    double t_step, t_max;
    bool directioninitialed;
    int transparent;
    float xpos,ypos,zpos,xmin,xmax,ymin,ymax,zmin,zmax,xrot,yrot,zrot;
    float bottom, top, right, left, near;
    float xtrans,ytrans,ztrans;
    float xscale,yscale,zscale;
    bool rotate, zoom;
    bool direction;
    bool equationchanged;
    int DynamicTime;
    bool dynamic;
};


#endif // ZONOVIEW_H
