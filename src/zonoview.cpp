#include "zonoview.h"
//#include "ui_mainwindow.h"
//#include <ui_mainwindow.h>
#include <QMouseEvent>
#include <QColorDialog>
#include <GL/glu.h>
#include <cstdlib>
#include <math.h>
#include <QThread>
#include <QFormLayout>
#include "func.h"
#include <QTableWidget>
#include <QLayout>
#include <QPushButton>
#include "zonotope.h"
#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QMetaType>

/**
 * @brief Constructor of the GraphicalInterface class
 * @param parent
 */

void ZonoView::startStatic(){
    dynamic = false;
    initColor(MAX_FACE);
    if (equationchanged){
        Zonotope zp = Zonotope(ex,ey,ez);
        vf = zp.getAllFace();
        setface(vf);
        equationchanged = false;
    }
    initializeGL();
    show();
    updateGL();
}

void ZonoView::startDynamic(){
    dynamic = true;
    directioninitialed = false;

    Zonotope zp = Zonotope(ex, ey, ez, true);
    zp.setmaxtime(t_max);
    zp.settimestep(t_step);

    initColor(MAX_FACE);

    if (t_step <= 0 || t_max < t_step || t_max / t_step > MAX_STEP){
        startStatic();
        return;
    }

    CalcThread *cthread = new CalcThread(zp);
    QObject::connect(cthread, SIGNAL(faceChanged(int *)),
                     this, SLOT(faceChanged(int *)));
    QObject::connect(this, SIGNAL(updated()), cthread, SLOT(faceupdated()));
    cthread->start();
}

void ZonoView::setDefaultEquation(QString ex, QString ey, QString ez){
    this->ex = ex;
    this->ey = ey;
    this->ez = ez;
    transparent = 255;
    equationchanged = true;
}

void ZonoView::initColor(int numberofface){
    faceColors = new QColor[numberofface];
    for(int i = 0; i < numberofface; i++){
        faceColors[i] = QColor(rand()%255,rand()%255,rand()%255, transparent);
    }
}

void ZonoView::setface(vector<Face> vfin){
    // Add the face vector

    vf = vfin;
    int vfsize = vf.size();
    if (vfsize <= 3)
        return;

    // Create random colors for each face
    if (vfsize > MAX_FACE)
        initColor(vfsize);

    // Find the range of the coordinate
    Point potemp = vf[0].getvp()[0];
    xmin = xmax = potemp.x();
    ymin = ymax = potemp.y();
    zmin = zmax = potemp.z();
    for (int i = 0; i < vfsize; i++){
        Face ftemp = vf[i];
        int fsize =ftemp.getsize();
        vector<Point> vpotemp = ftemp.getvp();
        for (int j = 0; j < fsize; j++){
            potemp = vpotemp[j];
            if (potemp.x() < xmin) xmin = potemp.x();
            if (potemp.x() > xmax) xmax = potemp.x();
            if (potemp.y() < ymin) ymin = potemp.y();
            if (potemp.y() > ymax) ymax = potemp.y();
            if (potemp.z() < zmin) zmin = potemp.z();
            if (potemp.z() > zmax) zmax = potemp.z();
        }
    }
    xtrans = (xmin-xmax)/2.0 -xmin;
    ytrans = (ymin-ymax)/2.0 -ymin;
    ztrans = (zmin-zmax)/2.0 -zmin;
    xscale=yscale=zscale=1;
}

ZonoView::ZonoView(QWidget *parent)
    : QGLWidget(parent)
{
    ex = "1+2u-v+3w";
    ey = "t-u-v+w";
    ez = "-1+s+t-w";
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
}

void ZonoView::initializeGL()
{

    qglClearColor(Qt::black);
    glLineWidth(1.5f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    glEnable(GL_LESS);
    bottom = -1.0;
    top = 1.0;
    left = -1.0;
    right = 1.0;
    near = 1.0;
    initView();
}

void ZonoView::initView(){
    // Initialize the view

    direction = true;
    int vfsize = vf.size();
    double fovy = 2.0 * atan((bottom-top) * 0.5 / near);
    double fovx = 2.0 * atan((right-left) * 0.5 /near);
    double thetay = fovy / 2.0;
    double thetax = fovx / 2.0;
    double zx, zy, ytemp, xtemp, ztemp;
    zpos = 0;
    Point potemp = vf[0].getvp()[0];
    for (int i = 0; i < vfsize; i++){
        Face ftemp = vf[i];
        int fsize = ftemp.getsize();
        vector<Point> vpotemp = ftemp.getvp();
        for (int j = 0; j < fsize; j++){
            potemp = vpotemp[j];
            ytemp = ABS(potemp.y()) + 1.0;
            zy = ytemp / tan(thetay);
            xtemp = ABS(potemp.x()) + 1.0;
            zx = xtemp / tan(thetax);
            ztemp = MAX(1.0, MAX(zx,zy));
            ztemp += potemp.z();
            zpos = MIN(zpos, -ztemp);
        }
    }
    xpos = ypos = 0;
    if (!dynamic || !directioninitialed){
        xrot = yrot = zrot = 0;
        directioninitialed = true;
    }
}

void ZonoView::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    top = -1.0*height/width;
    bottom = 1.0*height/width;
    left = -1.0;
    right = 1.0;
    near = 1.0;
    glFrustum(left, right, top, bottom, near, 2000.0);
    glMatrixMode(GL_MODELVIEW);
}

void ZonoView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}

void ZonoView::draw()
{
    // Draw the zonotope

    bool isprint = false;// Whether print the drawing information

    glPushMatrix();
    glTranslatef(xpos,ypos,zpos);
    glRotatef(xrot,0.0,1.0,0.0);
    glRotatef(yrot,cos(RAD(xrot)), 0.0, sin(RAD(xrot)));
    glTranslatef(xtrans*xscale, ytrans*xscale, ztrans*xscale);
    glScalef(xscale,yscale,zscale);

    int size = vf.size();
    for (int i = 0; i < size; i++){
        if (isprint)
            qDebug("The %d face",i);
        Face ftemp = vf[i];
        vector<Point> vpotemp = ftemp.getvp();
        int psize = vpotemp.size();
        glLoadName(i);
        glBegin(GL_TRIANGLE_FAN);
        qglColor(faceColors[i]);
        for (int k = 0; k < psize; k++){
            if (isprint)
                qDebug("%.2f,%.2f,%.2f",vpotemp[k].x(),vpotemp[k].y(),vpotemp[k].z());
            glVertex3f(vpotemp[k].x(),vpotemp[k].y(),vpotemp[k].z());
        }
        glEnd();
    }
    glPopMatrix();
}

void ZonoView::mousePressEvent(QMouseEvent *event)
{
    // Define mouse press action
    // Right button: Rotate

    if (event->button() == Qt::RightButton){
        lastPos = event->pos();
        rotate = true;
    }
}

void ZonoView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton){
        rotate = false;
        zoom = false;
    }
}

void ZonoView::mouseMoveEvent(QMouseEvent *event)
{
    // Define mouse move action

    double deltax, deltay;
    deltax = event->x() - lastPos.x();
    deltay = (event->y() - lastPos.y());

    if (rotate){
        const double drotx = 360.0 * deltax / 1000.0;
        const double droty = 360.0 * deltay / 1000.0;

        yrot += droty;

        if (ABS(yrot) > 90 && ABS(yrot) < 180)
            xrot -= drotx;
        else
            xrot += drotx;

        while (xrot < -180) xrot += 360;
        while (xrot > 180) xrot -= 360;
        if (yrot > 180) {yrot -= 360; direction = !direction;}
        if (yrot < -180) {yrot += 360; direction = !direction;}
        updateGL();
    }
    lastPos = event->pos();
}

void ZonoView::wheelEvent(QWheelEvent *event){
    int numDegreees = event->delta();

    if (numDegreees > 0){
        xscale +=ZOOMSPEED;
        yscale +=ZOOMSPEED;
        zscale +=ZOOMSPEED;
    }
    else if (numDegreees < 0){
        xscale -=ZOOMSPEED;
        yscale -=ZOOMSPEED;
        zscale -=ZOOMSPEED;
        xscale = xscale > 0? xscale:0;
        yscale = yscale > 0? yscale:0;
        zscale = zscale > 0? zscale:0;
    }

    if (xscale > 1)
        emit scaleChanged(int((xscale-1)*100.0/MAX_SCALE));
    else
        emit scaleChanged(int((xscale-1)*100.0/MIN_SCALE));

    updateGL();
}
