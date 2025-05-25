#include "widget.h"
#include "ui_widget.h"
#include<QHostInfo>
#include<QDebug>
#include<QDateTime>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    TCP_server = new QTcpServer();
    client_socket=nullptr;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btn_start_clicked()
{
    ui->pte_show->appendPlainText("开始监听。。。\n");
    TCP_server->listen(QHostAddress::Any,8080);
    connect(TCP_server,&QTcpServer::newConnection,this,&Widget::slot_newConnetion);
}

void Widget::slot_newConnetion()
{
    if(TCP_server->hasPendingConnections())  //查询是否有新连接
    {
        client_socket = TCP_server->nextPendingConnection(); //获取与真实客户端相连的客户端套接字
        ui->pte_show->appendPlainText("client login!"); //若有新连接，则提示

        this->connect(client_socket,SIGNAL(readyRead()),this,SLOT(slot_getData())); //连接客户端的套接字的有新消息信号到接收消息的槽
        this->connect(client_socket,SIGNAL(disconnected()),this,SLOT(slot_disconnect())); //连接客户端的套接字取消连接信号到取消连接槽
    }
}
//获取数据
void Widget::slot_getData()
{
    if(client_socket != nullptr)
    {
        QByteArray array = client_socket->readAll();    //接收消息
        QHostAddress clientaddr = client_socket->peerAddress(); //获得IP
        int port = client_socket->peerPort();   //获得端口号

        QDateTime datetime = QDateTime::currentDateTime();
        QString getMsg = tr("收到数据:") +clientaddr.toString() + tr(" : ") \
                              + QString::number(port) + tr("   ") + datetime.toString("yyyy-M-dd hh:mm:ss") + tr("\n");
        getMsg += array;
        ui->pte_show->appendPlainText(getMsg);
        gameManager.readData(array);
        slot_sendData();
    }
}
void Widget::slot_sendData(){
    QString sendMsg=gameManager.returnData();
    client_socket->write(sendMsg.toUtf8());
    QDateTime datetime = QDateTime::currentDateTime();
    QHostAddress clientaddr = client_socket->peerAddress(); //获得IP
    int port = client_socket->peerPort();   //获得端口号
    sendMsg =tr("发送数据:") + clientaddr.toString() + tr(" : ") \
                     + QString::number(port) + tr("   ") + datetime.toString("yyyy-M-dd hh:mm:ss") + tr("\n")+sendMsg;
    ui->pte_show->appendPlainText(sendMsg);
}
void Widget::slot_disconnect(){
    if (client_socket != nullptr) {
        // 确保客户端套接字断开连接
        if (client_socket->state() == QAbstractSocket::ConnectedState) {
            client_socket->close();
            client_socket->deleteLater();
        }

        // 记录日志
        ui->pte_show->appendPlainText("client logout!");

        // 立即删除对象并置空指针
        delete client_socket;
        client_socket = nullptr;
    }
}
void Widget::on_btn_stop_clicked()
{
    disconnect(client_socket,SIGNAL(disconnected()),this,SLOT(slot_disconnect()));
    if (TCP_server && TCP_server->isListening()) {
        TCP_server->close();
        delete TCP_server;
        TCP_server = nullptr;
        ui->pte_show->appendPlainText("Server stopped!");
    }
    QApplication::quit();
}
