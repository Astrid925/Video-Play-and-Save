#include "myslider.h"
#include <QDebug>
#include <QCoreApplication>
#include <QEvent>

mySlider::mySlider(QWidget*parent):QSlider(parent)
{
    this->setMouseTracking(true);

}

mySlider::~mySlider()
{

}

void mySlider::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);
    double rate=(double)event->pos().x()/(double)width();  //鼠标点击的位置，在整个weight的位置比率
    setValue(qRound64(rate*this->maximum()));
    emit sliderClicked(rate);
    QEvent mEvent(static_cast<QEvent::Type>(QEvent::User+1));
    QCoreApplication::sendEvent(parentWidget(),&mEvent);

}
