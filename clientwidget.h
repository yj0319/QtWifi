#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include<QUdpSocket>

namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = 0);
    ~ClientWidget();
    void  Udptext();  //发送消息
    void  readmsag(); //处理消息

private slots:
    void on_buttonConnect_clicked();

    void on_sendBtn_clicked();

private:
    Ui::ClientWidget *ui;

    QTcpSocket *tcpSocket;

    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 recvSize; //已经接收文件的大小

    bool isStart;   //标志位，是否为头部信息

    //利用UDP通信来发送文字
    QUdpSocket *udpSocket; //UDP套接字
};

#endif // CLIENTWIDGET_H
