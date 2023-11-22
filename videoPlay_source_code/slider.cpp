#include "slider.h"

slider::slider(QWidget *p):QSlider(p)
{

}

slider::~slider()
{

}

void slider::mousePressEvent(QMouseEvent *event)
{

    QSlider::mousePressEvent(event);
    QPoint pos=this->pos();
    double rate= (double)pos.x()/(double)width();
    setValue(rate*this->maximum());
    emit sliderClicked();
}
