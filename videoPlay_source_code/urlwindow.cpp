#include "urlwindow.h"
#include "ui_urlwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QTextCodec>

urlWindow::urlWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::urlWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString ("输入URL和保存路径"));
    // 复选框设置

    QString exePath=QCoreApplication::applicationDirPath();
    QString iniFile=exePath+"/local.ini";  //配置文件为可执行程序所在的目录下的local.ini
    m_IniFile = new QSettings(iniFile, QSettings::IniFormat);   //m_IniFile为全局变量QSettings *m_IniFile;
    m_IniFile->setIniCodec(QTextCodec::codecForName("UTF-8"));       //文件中的编码要设置和这里对应，比如我都设置成UTF-8

    QString url =m_IniFile->value("url/urlList").toString();   //通过Value函数将节下相对应的键值读取出来
    m_urlList=url.split("|");
    ui->urlEdit->setEditable(true);
    ui->urlEdit->setCurrentText(m_urlList.at(0));
    ui->urlEdit->addItems(m_urlList);

    QString folder =m_IniFile->value("folder/folderList").toString();   //通过Value函数将节下相对应的键值读取出来
    m_folderList=folder.split("|");
    ui->outRuteEdit->setEditable(true);
    ui->outRuteEdit->setCurrentText(m_folderList.at(0));
    ui->outRuteEdit->addItems(m_folderList);

}

urlWindow::~urlWindow()
{
    delete ui;
}

void urlWindow::on_playBnt_clicked()
{
    emit getStr(ui->urlEdit->currentText(),ui->outRuteEdit->currentText());

   // 复选框设置
    QString urlContent=ui->urlEdit->currentText();
    if (m_urlList.contains(urlContent))
    {
       m_urlList.removeAt(m_urlList.indexOf(urlContent));
    }
   m_urlList.push_front(urlContent);
   m_urlList.removeAll("");
   QString m_urlName;
   if(m_urlList.size()>10) // 最多保存10个
   {
      urlMaxNum=10;
   }
   else
   {
       urlMaxNum=m_urlList.size();
   }
   for (int i=0;i<urlMaxNum;i++ )
   {
        if(i== urlMaxNum-1)
        {
           m_urlName+=m_urlList.at(i);
        }
        else
        {
           m_urlName+=m_urlList.at(i)+"|";
        }
    }
    m_IniFile->setValue("url/urlList",m_urlName);

    //
    QString folderContent=ui->outRuteEdit->currentText();

    if (m_folderList.contains(folderContent))
    {
        m_folderList.removeAt(m_folderList.indexOf(folderContent));

    }
    m_folderList.push_front(folderContent);
    m_folderList.removeAll("");
    QString m_folderName;
    if(m_folderList.size()>10)
    {
        floderMaxNum=10;
    }
    else
    {
        floderMaxNum=m_folderList.size();
    }
    for (int i=0;i<floderMaxNum;i++ )
    {
         if(i==floderMaxNum-1)
         {
            m_folderName+=m_folderList.at(i);
         }
         else
         {
            m_folderName+=m_folderList.at(i)+"|";
         }
     }

    m_IniFile->setValue("folder/folderList",m_folderName);
    // 关闭当前窗口
    this->close();

}

void urlWindow::on_folderOptionBnt_clicked()
{
    QString folderName=QFileDialog::getExistingDirectory(this,tr("选择文件夹"),QDir::currentPath());
    if(!folderName.isEmpty())
    {
        ui->outRuteEdit->setCurrentText(folderName);
    }
}
