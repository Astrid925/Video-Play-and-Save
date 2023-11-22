#ifndef PLAYTHREAD_H
#define PLAYTHREAD_H

#include <QElapsedTimer>
#include <QThread>
#include "videoPlayBack.h"
#include <QTime>

class PlayThread : public QThread
{
    Q_OBJECT

public:
    explicit PlayThread(QObject *parent = nullptr);
    ~PlayThread() override;

    void open(QString strFile);  // 打开视频
    void pause(bool flag);       // 暂停视频
    void continuePlay (bool flag);  // 继续播放
    void close();                               // 关闭视频
    void seekFrame(double pos);

    void onSliderPressed();
    void onSliderReleased();


protected:
    void run() override;

signals:
    void repaint(AVFrame* frame);      // 将读取到的视频图像发送出去
    void videoTotalTime(qint64 totalTime);
    void videoPts(qint64 pts);
    void timeRate(double rate);



private:
    videoPlayBack *videoPlayMnger=nullptr;        // 视频解码类
    QString m_strFile="";                           // 打开文件
    bool m_play   = false;                      // 播放控制
    bool m_pause  = false;                      // 暂停控制
//    QElapsedTimer    m_etime1;            // 控制视频播放速度（更精确，但不支持视频后退）
};

#endif // PLAYTHREAD_H
