#include <QApplication>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include "zonoview.h"
#include "zonotope.h"
#include "point.h"
#include <QtOpenGL/QGLWidget>
#include <iostream>
#include <vector>
#include "facet.h"
#include "vertex.h"
#include "vertexlist.h"
#include "point3d.h"
#include <QtGlobal>
#include "quickhull3d.h"
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <QToolBar>
#include <QDockWidget>
#include <QSlider>
#include "MyQSlider.h"
#include <QSpinBox>
#include "mainwindow.h"
#include <QIcon>
#include <QElapsedTimer>
#include <QDebug>
#include <time.h>
#include <sys/time.h>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!QGLFormat::hasOpenGL()) {
    std::cerr << "This system has no OpenGL support" << std::endl;
    return 1;
    }

    MainWindow *window  = new MainWindow;
    window->show();
    return a.exec();
}



