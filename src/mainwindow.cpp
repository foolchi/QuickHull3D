#include "mainwindow.h"


/**
 * @brief Constructor of the main window : sets the window's geometry.
 * @param Pointer of parent widget, empty if no parent widget.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    /*! Setting of the title and of the size of the window */
    this->setWindowTitle("Zonotopes 3D") ;
    this->showMaximized() ;

    /*! Creation of a subsection LeftDockWidget in the window where there will be 3 tabs to enter the zonotope equations (2 cases : static or dynamic) and to choose the parameters. */
    QDockWidget * entryWindow = new QDockWidget() ;
    QTabWidget* entryTabs = new QTabWidget() ;
    QWidget* staticTab = new QWidget() ;
    QWidget* dynamicTab = new QWidget() ;
    QWidget* parametersTab = new QWidget ;

    ZonoView *zonoview = new ZonoView();
    zonoview->setDefaultEquation("a","b","c");
    zonoview->startStatic();


    /*! Implementation of the tab dedicated to STATIC zonotopes. */
    /*! Generation of a layout with 3 dialog boxes - 1 for each equation (x, y, z) - and a "Ok" button to run the program. */
    QFormLayout* staticEquations = new QFormLayout ;

    QLineEdit* x = new QLineEdit() ;
    staticEquations->addRow("&x =", x);
    x->setMinimumSize(200,30);
    QObject::connect(x, SIGNAL(textChanged(QString)), zonoview, SLOT(setX(QString)));

    QLineEdit* y = new QLineEdit() ;
    staticEquations->addRow("&y =", y);
    y->setMinimumSize(200,30);
    QObject::connect(y, SIGNAL(textChanged(QString)), zonoview, SLOT(setY(QString)));

    QLineEdit* z = new QLineEdit() ;
    staticEquations->addRow("&z =", z);
    z->setMinimumSize(200,30);
    QObject::connect(z, SIGNAL(textChanged(QString)), zonoview, SLOT(setZ(QString)));

    QPushButton* runStaticProgram = new QPushButton("Ok") ;
    runStaticProgram->setMinimumSize(200,30);

    QObject::connect(runStaticProgram, SIGNAL(clicked()),zonoview , SLOT(startStatic())) ;
    staticEquations->addWidget(runStaticProgram) ;

    QTextBrowser *statictips = new QTextBrowser();
    statictips->setMinimumSize(200,30);
    statictips->setText("Usage:\nFor each equation, input a combination of coefficients and caracters(a-z,A-Z).\nFor example:\nx=a,y=b,z=c is a cube.\nAnother example:\nx=1+2u-v+3w\ny=t-u-v+w\nz=1.1s+1.2t-0.9w+0.1\nAuthor:\nQi FU:qfu@ensta.fr");
    staticEquations->addWidget(statictips);

    staticTab->setLayout(staticEquations) ;
    staticTab->setGeometry(0,0,10,10) ;
    entryTabs->addTab(staticTab, "Static") ;
    staticTab->show();

    /*! Implementation of the tab dedicated to DYNAMIC zonotopes. */
    /*! Generation of a layout with 5 dialog boxes (x, y, z, t_max, t_step) and a "Ok" button to run the program. */
    QFormLayout* dynamicEquations = new QFormLayout ;

    QLineEdit* x_t = new QLineEdit() ;
    dynamicEquations->addRow("&x =", x_t);
    x_t->setMinimumSize(200,30);
    QObject::connect(x_t, SIGNAL(textChanged(QString)), zonoview, SLOT(setX(QString)));

    QLineEdit* y_t = new QLineEdit() ;
    dynamicEquations->addRow("&y =", y_t);
    y_t->setMinimumSize(200,30);
    QObject::connect(y_t, SIGNAL(textChanged(QString)), zonoview, SLOT(setY(QString)));

    QLineEdit* z_t = new QLineEdit() ;
    dynamicEquations->addRow("&z =", z_t);
    z_t->setMinimumSize(200,30);
    QObject::connect(z_t, SIGNAL(textChanged(QString)), zonoview, SLOT(setZ(QString)));

    QLineEdit* t_max = new QLineEdit() ;
    dynamicEquations->addRow("tmax =", t_max) ;
    t_max->setMinimumSize(200,30);
    QObject::connect(t_max, SIGNAL(textChanged(QString)), zonoview, SLOT(setTmax(QString)));

    QLineEdit* t_step = new QLineEdit() ;
    dynamicEquations->addRow("tstep =", t_step) ;
    t_step->setMinimumSize(200,30);
    QObject::connect(t_step, SIGNAL(textChanged(QString)), zonoview, SLOT(setTstep(QString)));

    QPushButton* runDynamicProgram = new QPushButton("Ok") ;
    runDynamicProgram->setMinimumSize(200,30);
    QObject::connect(runDynamicProgram, SIGNAL(clicked()), zonoview, SLOT(startDynamic())) ;
    dynamicEquations->addWidget(runDynamicProgram) ;

    QTextBrowser *dynamictips = new QTextBrowser();
    dynamictips->setMinimumSize(200,30);
    dynamictips->setText("Usage (Dynamic Mode):\nFor each equation, input a combination of coefficients and caracters(a-z,A-Z).\nFor dynamic variables, using variable t and brakets.\nFor example:\nx=(10t+0.1)a\ny=(1+t^2)b\nz=(0.1)a+(0.9t^3+1)c+d\nThe time t varies from 0 to tmax, changing the convex for each tstep.\nThe maximum exponent of t is 9.\nAuthor:\nQi FU:qfu@ensta.fr");
    dynamicEquations->addWidget(dynamictips);

    dynamicTab->setLayout(dynamicEquations) ;
    dynamicTab->setGeometry(0,0,10,10) ;
    entryTabs->addTab(dynamicTab, "Dynamic") ;
    dynamicTab->show();



    /*! Implementation of the tab dedicated to the display parameters : zoom and rotation. */
    QFormLayout* zoomLayout = new QFormLayout(parametersTab) ;
    zoomLayout->setAlignment(Qt::AlignCenter) ;

    /*! You can set the zoom thanks to a spin box or to a slider */
    QSpinBox* zoom = new QSpinBox() ;
    zoom->setFixedSize(200,30);
    zoom->setRange(-100,100);
    zoom->setPrefix("Zoom :                       ");
    zoom->setAlignment(Qt::AlignLeft);
    zoomLayout->addWidget(zoom);

    QSlider* zoomSlider = new QSlider(Qt::Horizontal) ;
    zoomSlider->setFixedSize(200,20);
    zoomSlider->setRange(-100,100) ;
    zoomSlider->setSliderPosition(0);
    zoomSlider->setTickPosition(QSlider::TicksBelow);
    zoomSlider->setTickInterval(50);
    zoomLayout->addWidget(zoomSlider);

    QObject::connect(zoom,SIGNAL(valueChanged(int)), zoomSlider, SLOT(setValue(int))) ;
    QObject::connect(zoomSlider, SIGNAL(valueChanged(int)), zoom, SLOT(setValue(int))) ;
    QObject::connect(zoom, SIGNAL(valueChanged(int)), zonoview, SLOT(setScale(int)));
    QObject::connect(zonoview, SIGNAL(scaleChanged(int)), zoom, SLOT(setValue(int)));


    /*! You can rotate the zonotope by pressing the button corresponding to the wanted direction. */
    QPushButton* rightRotate = new QPushButton(">", parametersTab) ;
    rightRotate->setGeometry(160,205,30,30);
    QObject::connect(rightRotate, SIGNAL(pressed()),zonoview ,SLOT(rotateright())) ;
    QObject::connect(rightRotate, SIGNAL(released()), zonoview, SLOT(rotatestop()));

    QPushButton* leftRotate = new QPushButton("<", parametersTab) ;
    leftRotate->setGeometry(50,205,30,30);
    QObject::connect(leftRotate, SIGNAL(pressed()), zonoview,SLOT(rotateleft()));
    QObject::connect(leftRotate, SIGNAL(released()), zonoview, SLOT(rotatestop()));

    QPushButton* upRotate = new QPushButton("Up", parametersTab) ;
    upRotate->setGeometry(100,150,40,40);
    QObject::connect(upRotate, SIGNAL(pressed()), zonoview,SLOT(rotateup())) ;
    QObject::connect(upRotate, SIGNAL(released()), zonoview, SLOT(rotatestop()));

    QPushButton* downRotate = new QPushButton("down", parametersTab) ;
    downRotate->setGeometry(100,250,40,40);
    QObject::connect(downRotate, SIGNAL(pressed()), zonoview,SLOT(rotatedown())) ;
    QObject::connect(downRotate, SIGNAL(released()), zonoview, SLOT(rotatestop()));


    parametersTab->setLayout(zoomLayout) ;
    parametersTab->setGeometry(0,0,10,10) ;
    entryTabs->addTab(parametersTab, "Parameters") ;
    parametersTab->show() ;


    entryWindow->setWidget(entryTabs);
    this->addDockWidget(Qt::LeftDockWidgetArea, entryWindow, Qt::Horizontal);



    /*! Creation of a tool bar to mainly have a button to save the zonotope as a picture. */
    QToolBar *toolbar = new QToolBar;
    toolbar->addSeparator();

    QPushButton *saveButton = new QPushButton("Save your zonotope");
    saveButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolbar->addWidget(saveButton);
    QObject::connect(saveButton, SIGNAL(clicked()), zonoview, SLOT(savePicture()));

    this->addToolBar(toolbar);


    /*! Creation of the central widget which displays the 3D zonotope. */
    this->setCentralWidget(zonoview);

};
