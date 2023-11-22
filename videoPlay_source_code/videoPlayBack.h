#ifndef VIDEOPLAYBACK_H
#define VIDEOPLAYBACK_H

#include <QObject>
#include <stdio.h>
#include <iostream>
#include <QTimer>
#include <QImage>
extern "C"
{
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "libavcodec/avcodec.h"
#include "libavcodec/bsf.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
#include "libavutil/log.h"
#include "libavutil/imgutils.h"
#include "libavutil/time.h"
#include <libswresample/swresample.h>
}

class videoPlayBack: public QObject
{
    Q_OBJECT
public:
    explicit videoPlayBack(QObject *parent = nullptr); // no parent
    ~videoPlayBack();

    void getRtspURL(QString strRtspURL); //获取RTSP URL
    bool open(); // 打开媒体文件，及建立输出文件
    AVFrame * read();                                       // 读取视频图像
    void close();                                        // 关闭读取和写操作
    bool isEnd();                                      // 是否到达文件末尾
    qint64 ptime();
    qint64 totalTime();
    void seekFrameTime(double pos);
    qint64 oneFrameTime();


private:

    QString m_qstrFile="";
    AVFormatContext *m_pInFmtCtx=nullptr;  //输入

    int nVideo_indx=0;
    AVCodecContext* pInCodecCtx=nullptr;  //获取解码器上下文
    AVPacket *packet=nullptr;
    AVFrame *pFrame=nullptr;

    bool m_End=false;
    qint64 m_pts=0;                    // 图像帧的显示时间
    qint64 m_totalTime=0;

    qreal sliderPos=0;
    double stamp=0;
    bool isSeekFrame=false;
    AVRational in_time_base1;
    qint64 in_duration=0;
    qint64 m_obtainFrames=0 ;                    // 视频当前获取到的帧数

};

#endif // VIDEOPLAYBACK_H
