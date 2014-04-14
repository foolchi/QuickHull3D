#ifndef GRAPHICALINTERFACE_H
#define GRAPHICALINTERFACE_H

#include <QMainWindow>
#include <QDockWidget>

namespace Ui {class GraphicalInterface;}

class GraphicalInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphicalInterface(QWidget *parent = 0);
    ~GraphicalInterface();


private:
    Ui::GraphicalInterface *m_ui;
};

#endif // GRAPHICALINTERFACE_H
