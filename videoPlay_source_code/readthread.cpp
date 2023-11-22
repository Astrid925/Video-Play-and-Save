#include "readthread.h"
#include <QEventLoop>
#include <QDebug>


ReadThread::ReadThread(QObject *parent) : QThread(parent)
{
   realTimeMnger = new realTimePlay;

}

ReadThread::~ReadThread()
{
  delete realTimeMnger;

}

/**
 * @brief      传入播放的视频地址并开启线程
 * @param url
 */
void ReadThread::open(QString url, QString strOut)
{
    if(!this->isRunning())
    {
        m_url = url;
        m_strOut=strOut;
        emit this->start();
    }
}

/**
 * @brief       控制暂停、继续
 * @param flag  true：暂停  fasle：继续
 */
void ReadThread::pause(bool flag)
{
    m_pause = flag;
}

void ReadThread::continuePlay(bool flag)
{
    m_pause=!flag;
}

/**
 * @brief 关闭播放
 */
void ReadThread::close()
{
    m_play = false;
    m_pause = false;
}

void ReadThread::saveFileSignal(bool saveFileFlag)
{
    m_saveFile=saveFileFlag;
}

/**
 * @brief      非阻塞延时
 * @param msec 延时毫秒
 */
void ReadThread:: sleepMsec(int msec)
{
    if(msec <= 0) return;
    QEventLoop loop;		//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();			//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}



void ReadThread::run()
{
    realTimeMnger->getRtspURL(m_url);
    realTimeMnger->getOutURL(m_strOut);
    bool openRet=realTimeMnger->open();
    emit saveFilePath(realTimeMnger->saveFileName());
    if(openRet)
    {
        m_play=true;
       // m_etime1.start();
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
            sleepMsec(500);
        }
        realTimeMnger->saveFile(m_saveFile);  // 判断是否开始存储视频
        AVFrame* frame=realTimeMnger->read();


        if(frame)
        {
            //sleepMsec(int(realTimeMnger->ptime()) - m_etime1.elapsed());  // 1倍速播放
            sleepMsec(int(realTimeMnger->oneFrameTime()));
            emit videoPts(realTimeMnger->ptime());
            emit repaint(frame);
        }
        else
        {
            if(realTimeMnger->isEnd())
            {
                break;
            }
            sleepMsec(10);
        }
    }
    qDebug()<<"视频播放结束！";
    realTimeMnger->close();
    quit();  // 退出线程事件循环

}
