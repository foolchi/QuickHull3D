#ifndef MYQSLIDER_H
#define MYQSLIDER_H
#include <QApplication>
#include <QSlider>

class MyQSlider: public QSlider
{
public:
    MyQSlider(Qt::Orientation orientation, QWidget *parent):
        QSlider(orientation, parent){
    }

public slots:
    void setValueFromString(QString value){
        setValue(value.toInt());
    }
};

#endif // MYQSLIDER_H
