#include "page_welcome.h"
#include "ui_page_welcome.h"
#include<QPropertyAnimation>
#include<QCoreApplication>
#include<QDebug>
#include<QMessageBox>

page_welcome::page_welcome(page *parent)
    : page(parent)
    , ui(new Ui::page_welcome)
{
    ui->setupUi(this);
    this->server_socket=new QTcpSocket();
    server_socket->abort();
    server_socket->connectToHost("127.0.0.1",8080);
    connect(server_socket,&QTcpSocket::connected,this,&page_welcome::slot_connected);
    connect(server_socket,&QTcpSocket::disconnected,this,&page_welcome::slot_disconnect);
    connect(server_socket,&QTcpSocket::readyRead,this,&page_welcome::slot_getData);

    ui->frame_move_left->setGeometry(220,0,220,440);
    ui->frame_move_right->setGeometry(440,0,220,440);

    connect(ui->frame_move_left,&frame_move::signal_hover,this,&page_welcome::on_frame_move_left_hovered);
    connect(ui->frame_move_right,&frame_move::signal_hover,this,&page_welcome::on_frame_move_right_hovered);
    connect(ui->btn_exit,&QPushButton::clicked,[=](){QCoreApplication::quit();});
}

page_welcome::~page_welcome()
{
    delete ui;
}

void page_welcome::on_frame_move_left_hovered()
{
    QPropertyAnimation*animation=new QPropertyAnimation(ui->frame_move_left,"geometry");
    if(ui->frame_move_left->x()<220){
        //如果左滑块在最左边
        if(ui->frame_move_right->x()<440){
            on_frame_move_right_hovered();
        }
        animation->setStartValue(QRect(0,0,220,440));
        animation->setEndValue(QRect(220,0,220,440));
    }else{
        //在中间
        animation->setStartValue(QRect(220,0,220,440));
        animation->setEndValue(QRect(0,0,220,440));
    }
    animation->setDuration(500);//500毫秒
    animation->start();
}

void page_welcome::on_frame_move_right_hovered()
{
    QPropertyAnimation*animation=new QPropertyAnimation(ui->frame_move_right,"geometry");
    if(ui->frame_move_right->x()>=440){
        //如果右滑块在最右边
        if(ui->frame_move_left->x()>=220){
            on_frame_move_left_hovered();
        }
        animation->setStartValue(QRect(440,0,220,440));
        animation->setEndValue(QRect(220,0,220,440));
    }else{
        //在中间
        animation->setStartValue(QRect(220,0,220,440));
        animation->setEndValue(QRect(440,0,220,440));
    }
    animation->setDuration(500);//500毫秒
    animation->start();
}

void page_welcome::on_btn_register_clicked()
{
    //获取用户名、密码
    QString username = ui->lineEdit_register_username->text();
    QString password = ui->lineEdit_regsiter_password->text();
    QString password_again=ui->lineEdit_register_passwordAgain->text();
    if(username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this,"有问题哦！","用户名或者密码为空！",QMessageBox::Yes);
    }
    else if(!(password==password_again)){
        QMessageBox::warning(this,"有问题哦！","两次输入密码不一致！",QMessageBox::Yes);
    }
    else{
        if(this->isconnection){
            QJsonObject jsonData;
            jsonData["contentType"]="register";
            jsonData["username"]=username;
            jsonData["password"]=password;
            QString sendMsg=page_welcome::jsonToString(jsonData);
            server_socket->write(sendMsg.toUtf8());
        }else{
            QMessageBox::warning(this,"提示","网不好呢");
        }
    }
}

void page_welcome::slot_connected(){
    this->isconnection=true;
}
void page_welcome::slot_getData(){
    QByteArray getMsg=this->server_socket->readAll();

}
void page_welcome::slot_disconnect(){
    QMessageBox::warning(this,"提示","连接断开了。。。");
    server_socket->close();
    server_socket->deleteLater();
}


