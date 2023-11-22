#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QSlider>
#include <QObject>
#include <QMouseEvent>

class mySlider:public QSlider
{

     Q_OBJECT
public:
    mySlider(QWidget*parent=nullptr);
    ~mySlider();
protected:
   void mousePressEvent(QMouseEvent *event) override;

signals:
   void sliderClicked(double mValue);

};

#endif // MYSLIDER_H
