#include "playthread.h"
#include <QEventLoop>
#include <QDebug>
#include <QTimer>

PlayThread::PlayThread(QObject *parent) : QThread(parent)
{
   videoPlayMnger = new videoPlayBack;

}

PlayThread::~PlayThread()
{
    delete videoPlayMnger;

}

/**
 * @brief      传入播放的视频地址并开启线程
 * @param url
 */
void PlayThread::open(QString strFile)
{
    if(!this->isRunning())
    {
        m_strFile=strFile;
        emit this->start();
    }
}

/**
 * @brief       控制暂停、继续
 * @param flag  true：暂停  fasle：继续
 */
void PlayThread::pause(bool flag)
{
    m_pause = flag;
}

void PlayThread::continuePlay(bool flag)
{
    m_pause = !flag;
}

/**
 * @brief 关闭播放
 */
void PlayThread::close()
{
    m_play = false;
    m_pause = false;

}

void PlayThread::seekFrame(double pos)
{
    videoPlayMnger->seekFrameTime(pos);

}

void PlayThread::onSliderPressed()
{
    m_pause=true;
}

void PlayThread::onSliderReleased()
{
    m_pause=false;
}


/**
 * @brief      非阻塞延时
 * @param msec 延时毫秒
 */
void  sleepMsec(int msec)
{
    if(msec <= 0) return;
    QEventLoop loop;		//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();			//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void PlayThread::run()
{
    videoPlayMnger->getRtspURL(m_strFile);
    bool openRet=videoPlayMnger->open();
    if(openRet)
    {
        m_play=true;
        emit videoTotalTime(videoPlayMnger->totalTime());


    }
    else
    {
        qWarning()<<"无法打开视频链接!";
    }
    // 循环读取视频图像
    while(m_play)
    {
        while (m_pause) // 暂停的情况
        {
            sleepMsec(200);
        }

       AVFrame* frame=videoPlayMnger->read();
        if(frame)
        {
           sleepMsec(int(videoPlayMnger->oneFrameTime()));
           emit repaint(frame);
           emit videoPts(videoPlayMnger->ptime());
           double rate=double_t (videoPlayMnger->ptime())/double_t(videoPlayMnger->totalTime());
           emit timeRate(rate);

        }
        else
        {
            if(videoPlayMnger->isEnd())
            {
                break;
            }
            sleepMsec(1);
        }
    }
    qDebug()<<"视频播放结束！";
    videoPlayMnger->close();
    quit();  //退出线程的事件循环

}
