#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    //发送消息
    Udptext();
    tcpSocket = new QTcpSocket(this);

    isStart = true;

    //ui->progressBar->setValue(0); //当前值

    setWindowTitle("客户端 端口：6666");

    connect(tcpSocket, &QTcpSocket::readyRead,
    [=]()
    {
        //取出接收的内容
        QByteArray buf = tcpSocket->readAll();

        if( isStart ==true)
        {//接收头
            isStart = false;
            //解析头部信息
//            QString buf = "hello##1024";
//            QString str = "hello##1024#mike";
//            str.section("##", 0, 0);

            //初始化
            //文件名
            fileName = QString(buf).section("##", 0, 0);
            //文件大小
            fileSize = QString(buf).section("##", 1, 1).toInt();
            recvSize = 0;   //已经接收文件大小
            qDebug()<<" 客户端接受得文件大小 41行"<<fileSize<<endl;

            //打开文件
            //关联文件名字
            file.setFileName(fileName);

            //只写方式，打开文件
            bool isOk = file.open(QIODevice::WriteOnly);
            if(false == isOk)
            {
                qDebug() << "WriteOnly error 49";

                tcpSocket->disconnectFromHost(); //断开连接
                tcpSocket->close(); //关闭套接字

                return; //如果打开文件失败，中断函数
            }

            //弹出对话框，显示接收文件的信息
            QString str1 = QString("接收的文件: [%1: %2kb]").arg(fileName).arg(fileSize/1024);
            QMessageBox::information(this, "文件信息", str1);

            //设置进度条
            ui->progressBar->setMinimum(0); //最小值
            ui->progressBar->setMaximum(fileSize/1024); //最大值
            ui->progressBar->setValue(0); //当前值

        }
        else //文件信息
        {

            qint64 len = file.write(buf);
     //       recvSize += len; //累计接收大小
            if(len>0)
            {
                recvSize+=len;
                qDebug() <<"recvSize接受文件的大小" <<recvSize;
            }

            //更新进度条
           ui->progressBar->setValue(recvSize/1024);

            if(recvSize == fileSize) //文件接收完毕
            {

                //先给服务发送(接收文件完成的信息)
                tcpSocket->write("file done");
                 file.close(); //关闭文件
                QMessageBox::information(this, "完成", "文件接收完成");

                //断开连接
                tcpSocket->disconnectFromHost();
                tcpSocket->close();

            }
       }

        }

    );

}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::on_buttonConnect_clicked()
{
    //获取服务器的ip和端口
    QString ip = ui->lineEditIP->text();
    quint16 port = ui->lineEditPort->text().toInt();

    //主动和服务器连接
    tcpSocket->connectToHost(QHostAddress(ip), port);

    isStart = true;

    //设置进度条
    ui->progressBar->setValue(0);
}

//发送消息  UDP
void  ClientWidget::Udptext()
{
    udpSocket=new QUdpSocket(this);

    udpSocket->bind(6666);

    //组播QQ群 确定绑定IPv4
    udpSocket->bind(QHostAddress::AnyIPv4, 9999);

    //加入某个组播 像QQ群
    //组播地址是D类地址
    udpSocket->joinMulticastGroup( QHostAddress("224.0.0.2") );
    //udpSocket->leaveMulticastGroup(); //退出组播

    connect(udpSocket,&QUdpSocket::readyRead,[=](){

        readmsag();

    });
}
 //处理消息
void  ClientWidget::readmsag()
{
    //读取对方发送的内容
    char buf[1024] = {0};
    QHostAddress cliAddr; //对方地址
    quint16 port;    //对方端口
    qint64 len = udpSocket->readDatagram(buf, sizeof(buf), &cliAddr, &port);
    if(len > 0)
    {
        //格式化 [192.68.2.2:8888]aaaa
        QString str = QString("[%1:%2] %3")
                .arg(cliAddr.toString())
                .arg(port)
                .arg(buf);
        //给编辑区设置内容
        ui->textEdit->setText(str);
    }
}

//发送消息
void ClientWidget::on_sendBtn_clicked()
{
    //先获取对方的IP和端口
    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();

    //获取编辑区内容
    QString str = ui->textEdit->toPlainText();

    //给指定的IP发送数据
    udpSocket->writeDatagram(str.toUtf8(), QHostAddress(ip), port);
}
