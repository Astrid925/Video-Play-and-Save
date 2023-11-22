#ifndef REALTIMEPLAY_H
#define REALTIMEPLAY_H

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

class realTimePlay: public QObject
{
    Q_OBJECT
public:
    explicit realTimePlay(QObject *parent = nullptr); // no parent
    ~realTimePlay();

    void getRtspURL(QString strRtspURL); //获取RTSP URL
    void getOutURL(QString strRute);   //设置输出位置
    bool open(); // 打开媒体文件，及建立输出文件
    AVFrame * read();                                       // 读取视频图像
    void close();                                        // 关闭读取和写操作
    bool isEnd();                                      // 是否到达文件末尾
    qint64 ptime();
    void saveFile(bool saveFlag);
    qint64 oneFrameTime();
    QString saveFileName();
private:
    bool setOutputCtx(AVCodecContext *encCtx, AVFormatContext **pTsFmtCt);
    qreal rationalToDouble(AVRational* rational);       // 将AVRational转换为double


private:

    QString m_qstrRtspURL="";
    QString m_qstrOutPutFile="";

    AVFormatContext *m_pInFmtCtx=nullptr;  //输入
    AVFormatContext *m_pTsFmtCtx=nullptr;  //文件操作Output

    int nVideo_indx=0;
    AVCodecContext* pInCodecCtx=nullptr;  //获取解码器上下文
    AVPacket *packet=nullptr;
    AVFrame *pFrame=nullptr;

    AVCodecContext *encCtx=nullptr; //输出编码器
    AVStream *out_stream=nullptr;

    bool m_End=false;
    bool m_writeHeader=false;
    qint64 m_pts=0;                    // 图像帧的显示时间

    bool m_saveFile=false;
    char*cPacketData=nullptr;
    uint8_t* cExtradata=nullptr;
    bool firstKeyFrame=true;

    AVRational in_time_base1;
    qint64 in_duration=0;

    QString m_saveFileName="";
};

#endif // REALTIMEPLAY_H
