#ifndef URLWINDOW_H
#define URLWINDOW_H

#include <QWidget>
#include <QSettings>
#include <QCompleter>

namespace Ui {
class urlWindow;
}

class urlWindow : public QWidget
{
    Q_OBJECT

public:
    explicit urlWindow(QWidget *parent = nullptr);
    ~urlWindow();


private slots:
    void on_playBnt_clicked();

    void on_folderOptionBnt_clicked();

signals:
    void getStr(QString url,QString outRute);
private:
    Ui::urlWindow *ui;
    QSettings *m_IniFile;
    QStringList m_urlList;
    QStringList m_folderList;
    QCompleter *urlCompleter;
    QCompleter *folderCompleter;

    qint8 urlMaxNum = 0;
    qint8 floderMaxNum = 0;

};

#endif // URLWINDOW_H
