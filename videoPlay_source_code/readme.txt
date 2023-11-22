这个项目实现了视频实时显示、存储，以及本地视频回放功能。视频的显示是采用GPU进行显示，且在GPU中完成了YUV数据转换为RGB格式数据。
以下是文件说明：
1. mainwindow 是UI主窗口文件
2.urlwindow 是UI子窗口文件，主要用于输入 URL、视频保存地址
3. playimage 是GPU显示文件，包含了YUV转RGB功能
4. playthread 是用于本地回放的线程文件
5. readthread  是用于视频实时显示、存储的线程文件
6.realTimePlay 是视频实时显示、存储的功能实现文件
7.videoPlayBack 是本地视频回放的功能实现文件

需要注意的点：realTimePlay、videoPlayBack文件的close函数，解析函数不同于其他项目；主要解决连续打开不同文件，内存不是null的问题。