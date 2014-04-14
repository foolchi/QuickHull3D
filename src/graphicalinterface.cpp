/**
 * Libraries and headers inclusion
 */
#include <QDesktopWidget>
#include <QInputDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QLayout>



#include "graphicalinterface.h"
#include "ui_graphicalinterface.h"


/**
 * @brief Constructor of the GraphicalInterface class
 * @param parent
 */
GraphicalInterface::GraphicalInterface(QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::GraphicalInterface)
{
    m_ui->setupUi(this);
    setWindowTitle("Zonotopes 3D"); // Title of the window
    showMaximized(); // Sets a full-screen window


    QLayout* entryArea = new QLayout(this) ;
    QLayout* equationsSystem = new QLayout(entryArea) ;
    QFormLayout* equations = new QFormLayout(equationsSystem) ;
        QLineEdit* x = new QLineEdit() ;
        equations->addRow("&x", x);
        QLineEdit* y = new QLineEdit() ;
        equations->addRow("&y", y);
        QLineEdit* z = new QLineEdit() ;
        equations->addRow("&z", z);
    QLayout* displayParameters = new QLayout(entryArea) ;
    // Creation of a form layout to enter the zonotope equations, included in the equationsSystem box








    // Creation of a left dock widget for zonotope equations and parameters
    QDockWidget* entryArea = new QDockWidget(tr("Zonotope equations & display parameters"), this);
    entryArea->setAllowedAreas(Qt::LeftDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, entryArea);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

    // Creation of an equationsSystem box that includes the equations and the validation button "ok"
    QListWidget* equationsSystem = new QListWidget(entryArea);



    // Inclusion of the equations into the equationsSystem box
    equationsSystem->addItem(equations);

    // Creation of the validation button, included in the equationsSystem box
    QPushButton* equationsValidation = new QPushButton("Ok") ;
    equationsSystem->addItem(equationsValidation) ;

    // Inclusion of the equationsSystem box into the entryArea
    entryArea->setWidget(equationsSystem);
}


/**
 * @brief Destructor
 */
GraphicalInterface::~GraphicalInterface()
{
    delete m_ui;
}














