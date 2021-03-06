#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer> //监听套接字
#include <QTcpSocket> //通信套接字
#include <QFile>
#include <QTimer>
#include<QUdpSocket>

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = 0);
    ~ServerWidget();

    void sendData(); //发送文件数据

    void udptext(); //udp发送消息

    void dealmsg();  //UDP处理信息

private slots:
    void on_buttonFile_clicked();

    void on_buttonSend_clicked();

    void on_sendBtn_clicked();

private:
    Ui::ServerWidget *ui;

    QTcpServer *tcpServer; //监听套接字
    QTcpSocket *tcpSocket; //通信套接字

    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 sendSize; //已经发送文件的大小

    QTimer timer; //定时器

    //利用UDP通信来发送文字
    QUdpSocket *udpSocket; //UDP套接字



};

#endif // SERVERWIDGET_H
