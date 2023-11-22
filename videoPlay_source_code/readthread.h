#ifndef READTHREAD_H
#define READTHREAD_H

#include <QElapsedTimer>
#include <QThread>
#include "realTimePlay.h"

class ReadThread : public QThread
{
    Q_OBJECT

public:
    explicit ReadThread(QObject *parent = nullptr);
    ~ReadThread() override;

    void open(QString url, QString strOut);  // 打开视频
    void pause(bool flag);                      // 暂停视频
    void continuePlay(bool flag );              // 继续播放
    void close();                               // 关闭视频
    void saveFileSignal(bool saveFileFlag);

   // const QString& url();                       // 获取打开的视频地址

protected:
    void run() override;

signals:
    void repaint(AVFrame* frame);               // 重绘
    void videoPts(qint64 pts);
    void saveFilePath(QString filePath);

private:
    void sleepMsec(int s);
    realTimePlay *realTimeMnger=nullptr;        // 视频解码类
    QString m_url="";                              // 打开的视频地址
    QString m_strOut="";                           // 打开文件
    bool m_play   = false;                      // 播放控制
    bool m_pause  = false;                      // 暂停控制
  //  QElapsedTimer m_etime1;                     // 控制视频播放速度（更精确，但不支持视频后退）
    bool m_saveFile=false;
};
#endif // READTHREAD_H
