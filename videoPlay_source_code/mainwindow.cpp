#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTimer>
#include <QMouseEvent>
#include "myslider.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString ("视频流软件"));
    this->setStyleSheet("MainWindow{font-size:20px}");
    // 使用QOpenGLWindow绘制

    playImage = new PlayImage;
#if USE_WINDOW
    ui->horizontalLayout_2->addWidget(QWidget::createWindowContainer(playImage));    // 这一步加载速度要比OpenGLWidget慢一点
#else
    ui->horizontalLayout_2->addWidget(playImage);
#endif
    m_readThread=new ReadThread;
    m_playThread=new PlayThread;
    subWindow=new urlWindow;
    connect(m_readThread, &ReadThread::repaint, playImage, &PlayImage::repaint, Qt::BlockingQueuedConnection);
    connect(subWindow, &urlWindow::getStr, this, &MainWindow::getURL_outRute);
    connect(m_playThread, &PlayThread::repaint, playImage, &PlayImage::repaint, Qt::BlockingQueuedConnection);

    // 视频播放时间显示
    connect(m_playThread,&PlayThread::videoTotalTime,this,&MainWindow::setTotalTime);
    connect(m_playThread,&PlayThread::videoPts,this,&MainWindow::setPlayTime);
    connect(m_readThread,&ReadThread::videoPts,this,&MainWindow::setPlayTime);
    // 滑动条移动
    connect(m_playThread,&PlayThread::timeRate,this, &MainWindow::setSliderValue);

    // 点击滑动条
    this->setMouseTracking(true);
    connect(ui->videoSlider, &mySlider::sliderClicked, this, &MainWindow::on_sliderClicked);

   //拖动滑动条
    connect(ui->videoSlider,SIGNAL(sliderPressed()),this,SLOT(on_sliderPressed()));
    connect(ui->videoSlider, SIGNAL(sliderMoved(int)), this,SLOT(on_sliderMoved(int)));
    connect(ui->videoSlider,SIGNAL(sliderReleased()),this, SLOT(on_sliderReleased()));

    //显示保存文件的路径
    connect(m_readThread,&ReadThread::saveFilePath,this,&MainWindow::on_saveFilePath);
}

MainWindow::~MainWindow()
{
    if(m_readThread)
    {
        m_readThread->close();
        m_readThread->terminate();
        delete m_readThread;
    }
    if(m_playThread)
    {
        m_playThread->close();
        m_playThread->terminate();
        delete m_playThread;
    }
    delete ui;
    delete subWindow;
}


void MainWindow::on_actOpenStream_triggered() // 弹出窗口
{

    subWindow->show();
}


void MainWindow::getURL_outRute(QString url, QString outRute) // 从子窗口获取链接
{
    m_url=url;
    m_outRute=outRute;
    this->update();
    m_readThread->open(m_url,m_outRute);

}

void MainWindow::on_actOpenFile_triggered()
{
    QString strName = QFileDialog::getOpenFileName(this, "选择播放视频", "/", "视频 (*.264*.265*.mp4 *.m4v *.mov *.avi *.flv);; 其它(*)");
    if(!strName.isEmpty())
    {
        m_playThread->open(strName);


    }

}


void MainWindow::on_actPlay_triggered()
{
    if (m_readThread)
    {
        m_readThread->continuePlay(true);
    }

    if (m_playThread)
    {
        m_playThread->continuePlay(true);
    }
}

void MainWindow::on_actPause_triggered()
{
    if (m_readThread)
    {
        m_readThread->pause(true);
    }

    if (m_playThread)
    {
        m_playThread->pause(true);
    }
}

void MainWindow::on_actStop_triggered()
{
    if (m_readThread)
    {
        m_readThread->close();
        playImage->clearPaint();

    }

    if (m_playThread)
    {
        m_playThread->close();
        playImage->clearPaint();

    }
}


void MainWindow::on_actSaveFile_triggered(bool checked)
{
    m_readThread->saveFileSignal(checked);
}

void MainWindow::setTotalTime(qint64 totalTime)
{
     m_totalTime=totalTime/1000; // 转换为秒
     qint64 min=m_totalTime/60;
     qint64 sec=m_totalTime%60;
     char buf[1024]={0};
     sprintf(buf,"%02d:%02d",min,sec);
     ui->totalTime->setText(buf);
}

void MainWindow::setPlayTime(qint64 pts)
{
    pts=pts/1000;   // 转换为秒
    qint64 min=pts/60;
    qint64 sec=pts%60;
    char buf[1024]={0};
    sprintf(buf,"%02d:%02d",min,sec);
    ui->playTime->setText(buf);
}

void MainWindow::setSliderValue(double rate)
{
    ui->videoSlider->setValue(rate*ui->videoSlider->maximum());
}


void MainWindow::on_sliderClicked(double rate)
{
    m_playThread->seekFrame(rate);
}

void MainWindow::on_sliderPressed()
{
    m_playThread->onSliderPressed();
}

void MainWindow::on_sliderMoved(int value)
{
    ui->videoSlider->setValue(value);
    double rate=(double)value/(double)ui->videoSlider->maximum();
    qint64 videoTime=rate*m_totalTime;
    qint64 min=videoTime/60;
    qint64 sec=videoTime%60;
    char buf[1024]={0};
    sprintf(buf,"%02d:%02d",min,sec);
    ui->playTime->setText(buf);

}

void MainWindow::on_sliderReleased()
{

    qint64 sliderPos=ui->videoSlider->value();
    double rate=(double)sliderPos/(double)ui->videoSlider->maximum();
    m_playThread->seekFrame(rate);
    m_playThread->onSliderReleased();

}

void MainWindow::on_saveFilePath(QString filePath)
{
    ui->actSaveFile->setToolTip(filePath);
}












