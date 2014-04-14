#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QtOpenGL/QGLWidget>
#include <iostream>
#include <vector>
#include <QtGlobal>
#include <cstdlib>
#include <QTextBrowser>
#include "zonoview.h"
#include <cstdio>
#include <time.h>
#include <QToolBar>
#include <QDockWidget>
#include <QSlider>
#include <QSpinBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    /**
     * @brief Constructor
     * @param Pointer of parent widget, empty if no parent widget
     */
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

};

#endif // MAINWINDOW_H
