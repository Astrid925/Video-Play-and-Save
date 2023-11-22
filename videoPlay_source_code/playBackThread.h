#ifndef PLAYTHREAD_H
#define PLAYTHREAD_H

#include <QElapsedTimer>
#include <QThread>
#include <QImage>
#include "videoPlayBack.h"

class ReadThread : public QThread
{
    Q_OBJECT
public:
    enum PlayState      // 视频播放状态
    {
        play,
        end
    };
public:
    explicit ReadThread(QObject *parent = nullptr);
    ~ReadThread() override;

    void open(QString strFile);  // 打开视频
    void pause(bool flag);                      // 暂停视频
    void close();                               // 关闭视频
   // const QString& url();                       // 获取打开的视频地址

protected:
    void run() override;

signals:
    void repaint(AVFrame* frame);      // 将读取到的视频图像发送出去
    void playState(PlayState state);            // 视频播放状态发送改变时触发

private:
    videoPlayBack *pFFmpegMnger=nullptr;        // 视频解码类
    QString m_strFile;                           // 打开文件
    bool m_play   = false;                      // 播放控制
    bool m_pause  = false;                      // 暂停控制
    QElapsedTimer    m_etime1;            // 控制视频播放速度（更精确，但不支持视频后退）
};


#endif // PLAYTHREAD_H
