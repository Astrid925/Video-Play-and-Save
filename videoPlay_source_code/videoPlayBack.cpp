#include "videoPlayBack.h"
#include <qdatetime.h>
#include <QDebug>

videoPlayBack::videoPlayBack(QObject *parent ):
    QObject(parent)
{




}

videoPlayBack::~videoPlayBack()
{
   close();

}

void videoPlayBack::getRtspURL(QString strFile)
{
    this->m_qstrFile = strFile;

}



bool videoPlayBack::open()
{

    if(m_qstrFile.isNull())
    {
        qDebug(" input url or outFile is null,===========keep trying \n");
        return false;
    }

    AVDictionary *options = nullptr;  // AVDictionary是键对存储工具
    av_dict_set(&options,"rtsp_transport", "tcp", 0); // 添加key-value对
    av_dict_set(&options, "max_delay", "3", 0);
    av_dict_set(&options,"timeout","10000000",0);
    av_dict_set(&options, "buffer_size", "1024000", 0);// 设置“buffer_size”缓存容量

    // 1.打开输入流并返回解封装上下文
    int nRet = avformat_open_input(&m_pInFmtCtx,               // 返回解封装上下文
                                  m_qstrFile.toStdString().c_str(),  // 打开视频地址
                                  nullptr,                   // 如果非null，此参数强制使用特定的输入格式。自动选择解封装器（文件格式）
                                  &options);                    // 参数设置
    if(options)
    {
        av_dict_free(&options); // 释放参数字典
    }
    if( nRet < 0)
    {

        qDebug("Fail to open the stream,===========keep trying \n");
        return false;
    }

    // 2.读取媒体文件的数据包以获取流信息。
    nRet=avformat_find_stream_info(m_pInFmtCtx, nullptr);
    if( nRet < 0)
    {
        qDebug("Fail to find the video stream information .\n");
        return false;
    }

    // 3.通过AVMediaType枚举查询视频流ID（也可以通过遍历查找），最后一个参数无用
    nVideo_indx = av_find_best_stream(m_pInFmtCtx,AVMEDIA_TYPE_VIDEO,-1,-1,nullptr,0);
    if(nVideo_indx < 0)
    {
        avformat_free_context(m_pInFmtCtx);
        qDebug("Fail to find a video stream.\n");
        return false;
    }

   if ( m_pInFmtCtx->duration !=AV_NOPTS_VALUE) //当 AVFormatContext 变量中存在duration字段时，可以采用该方式获取视频总时长
    {
         m_totalTime = m_pInFmtCtx->duration / (AV_TIME_BASE/1000); // 计算视频总时长（ms）
    }
    else if(m_pInFmtCtx->bit_rate !=0)
    {
        int64_t video_size=avio_size(m_pInFmtCtx->pb);   // 采用文件大小、比特率计算总时长
        int64_t video_bitrate=m_pInFmtCtx->bit_rate;
        m_totalTime=((8*video_size)/video_bitrate)*1000; // (ms)

    }
   else
   {
        m_totalTime=0;
   }

   AVStream* in_stream = m_pInFmtCtx->streams[nVideo_indx];
   in_time_base1 = in_stream->time_base; // 时间基
   in_duration =( (double)AV_TIME_BASE / av_q2d(in_stream->r_frame_rate))/1000; //一帧的时长

   // 4.通过解码器ID获取视频解码器
   const AVCodec *pInCodec = avcodec_find_decoder(m_pInFmtCtx->streams[nVideo_indx]->codecpar->codec_id);

   if( pInCodec==nullptr)
   {
       qDebug("Fail to find a decoder.\n");
       return false;
   }
   //5.获取解码器上下文
   pInCodecCtx = avcodec_alloc_context3(pInCodec);
   if(!pInCodecCtx)
   {
        qDebug("Fail to create the context of decoder.\n");
       return false;
   }

   //6.复制解码器参数, 使用视频流的codecpar为解码器上下文赋值
   nRet = avcodec_parameters_to_context(pInCodecCtx, m_pInFmtCtx->streams[nVideo_indx]->codecpar);
   if(nRet < 0)
   {
       avcodec_free_context(&pInCodecCtx);
       qDebug("Fail to copy the parameters.\n");
       return false;
   }

   //7.打开解码器
   nRet=avcodec_open2(pInCodecCtx, nullptr, nullptr);
   if(nRet < 0)
   {
       avcodec_free_context(&pInCodecCtx);
       qDebug("Error: Can't open decoder!\n");
       return false;
   }

   // 分配空间
   packet = av_packet_alloc();
   pFrame= av_frame_alloc();

   //
   m_End=false;
   return true;
}

AVFrame* videoPlayBack::read()    //读取一帧图像
{
    if(!m_pInFmtCtx) // 如果没有打开媒体文件
    {
        return nullptr;
    }


   // 读取帧数据
    int readRet=av_read_frame(m_pInFmtCtx, packet);
    if (readRet<0)
    {
        qDebug("到文件末尾了！");
        m_End=true;
        return nullptr;
    }
    else
    {
        if (isSeekFrame)
        {
            isSeekFrame=false;
            int seekRet=av_seek_frame(m_pInFmtCtx,nVideo_indx,(stamp*1000)/(av_q2d(in_time_base1)*AV_TIME_BASE),AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME);
            avcodec_flush_buffers(pInCodecCtx);  // 清除缓存
            if (seekRet<0)
            {
                qDebug("Fail to seek frame!");
                return nullptr;
            }

        }

       if (packet->stream_index == nVideo_indx)
       {
          m_pts=av_q2d(in_time_base1)*packet->pts*AV_TIME_BASE/1000; //ms
          if(avcodec_send_packet(pInCodecCtx, packet)<0)
          {
               qDebug("The input is to end.\n");
               return nullptr;
          }
       }
      av_packet_unref(packet);

      int got_picture = avcodec_receive_frame(pInCodecCtx, pFrame);//把解码好的YUV数据放到pFrame中

      if(0 == got_picture)//解码好一帧数据
      {
         // 对解码视频帧进行缩放、格式转换等操作

         return pFrame;
      }
      else
      {
          av_frame_unref(pFrame);
          return nullptr;
      }
    }

}

void videoPlayBack::close()
{

    if(m_pInFmtCtx && m_pInFmtCtx->pb)
    {
        avio_flush(m_pInFmtCtx->pb);
        avformat_free_context(m_pInFmtCtx);
        avcodec_free_context(&pInCodecCtx);
        av_packet_free(&packet);
        av_frame_free(&pFrame);
    }
    m_pInFmtCtx = nullptr;  //输入
    pInCodecCtx = nullptr;  //获取解码器上下文
    packet = nullptr;
    pFrame = nullptr;
    m_qstrFile = "";
    nVideo_indx = 0;
    m_End = false;
    m_pts = 0;
    m_totalTime=0;

    sliderPos=0;
    stamp=0;
    isSeekFrame=false;
    in_duration=0;

    m_obtainFrames=0;
}

bool videoPlayBack::isEnd()  //
{
    return m_End;
}

 qint64 videoPlayBack::ptime()
{
     return m_pts;
 }

 qint64 videoPlayBack::totalTime()
 {
     return m_totalTime;
 }

 void videoPlayBack::seekFrameTime(double pos)
 {

     stamp = pos*m_totalTime;
     isSeekFrame=true;

 }

 qint64 videoPlayBack::oneFrameTime()
 {
     return in_duration;
 }









