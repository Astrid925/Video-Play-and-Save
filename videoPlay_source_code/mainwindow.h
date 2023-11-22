#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "urlwindow.h"
#include "readthread.h"
#include "playimage.h"
#include "playthread.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//protected:
//    bool eventFilter(QObject*obj,QEvent*event) override;

private slots:
    void on_actOpenStream_triggered();

    void getURL_outRute(QString url, QString outRute);

    void on_actOpenFile_triggered();

    void on_actPlay_triggered();

    void on_actPause_triggered();

    void on_actStop_triggered();

    void on_actSaveFile_triggered(bool checked);

    void setTotalTime(qint64 totalTime);
    void setPlayTime(qint64 pts);
    void setSliderValue(double rate);

    void on_sliderClicked(double rate);

    void on_sliderPressed();
    void on_sliderMoved(int value);
    void on_sliderReleased();

    void on_saveFilePath(QString filePath);


private:
    Ui::MainWindow *ui;
    ReadThread *m_readThread=nullptr;
    PlayThread *m_playThread=nullptr;
    PlayImage* playImage = nullptr;
    urlWindow *subWindow=nullptr;
    QImage m_image;
    QString m_url="";
    QString m_outRute="";
    qint64 m_totalTime=0;


};
#endif // MAINWINDOW_H
