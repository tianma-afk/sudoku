#include "page_welcome.h"
#include "ui_page_welcome.h"
#include<QPropertyAnimation>
#include<QCoreApplication>
#include<QDebug>
#include<QMessageBox>
#include<QVBoxLayout>
#include<QMovie>
page_welcome::page_welcome(page *parent)
    : page(parent)
    , ui(new Ui::page_welcome)
{
    ui->setupUi(this);
    ui->lineEdit_login_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_register_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_register_passwordAgain->setEchoMode(QLineEdit::Password);


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
    float sec;
    QPropertyAnimation*animation=new QPropertyAnimation(ui->frame_move_left,"geometry");
    if(ui->frame_move_left->x()==0){
        //如果左滑块在最左边
        ui->frame_move_left->oren=frame_move::right;
        if(ui->frame_move_right->x()==220||(ui->frame_move_right->x()<440&&ui->frame_move_right->oren==frame_move::left)){
            on_frame_move_right_hovered();
        }
        animation->setStartValue(QRect(0,0,220,440));
        animation->setEndValue(QRect(220,0,220,440));
        sec=500;
    }else if(ui->frame_move_left->x()==220){
        //在中间
        ui->frame_move_left->oren=frame_move::left;
        animation->setStartValue(QRect(220,0,220,440));
        animation->setEndValue(QRect(0,0,220,440));
        sec=500;
    }else{
        //在运动中
        if(ui->frame_move_left->oren==frame_move::right){
            ui->frame_move_left->oren=frame_move::left;
            animation->setStartValue(QRect(ui->frame_move_left->x(),0,220,440));
            animation->setEndValue(QRect(0,0,220,440));
            sec=float(ui->frame_move_left->x()-0)/220.0*500;
        }else{
            ui->frame_move_left->oren=frame_move::right;
            if(ui->frame_move_right->x()==220||(ui->frame_move_right->x()<440&&ui->frame_move_right->oren==frame_move::left)){
                on_frame_move_right_hovered();
            }
            animation->setStartValue(QRect(ui->frame_move_left->x(),0,220,440));
            animation->setEndValue(QRect(220,0,220,440));
            sec=float(220-ui->frame_move_left->x())/220.0*500;
        }
    }
    animation->setDuration(sec);//最大500毫秒
    animation->start();
}

void page_welcome::on_frame_move_right_hovered()
{
    float sec;
    QPropertyAnimation*animation=new QPropertyAnimation(ui->frame_move_right,"geometry");
    if(ui->frame_move_right->x()==440){
        //如果右滑块在最右边
        ui->frame_move_right->oren=frame_move::left;
        if(ui->frame_move_left->x()==220||(ui->frame_move_left->x()>0&&ui->frame_move_left->oren==frame_move::right)){
            on_frame_move_left_hovered();
        }
        animation->setStartValue(QRect(440,0,220,440));
        animation->setEndValue(QRect(220,0,220,440));
        sec=500;
    }else if(ui->frame_move_right->x()==220){
        //在中间
        ui->frame_move_right->oren=frame_move::right;
        animation->setStartValue(QRect(220,0,220,440));
        animation->setEndValue(QRect(440,0,220,440));
        sec=500;
    }else{
        if(ui->frame_move_right->oren==frame_move::right){
            ui->frame_move_right->oren=frame_move::left;
            if(ui->frame_move_left->x()==220||(ui->frame_move_left->x()>0&&ui->frame_move_left->oren==frame_move::right)){
                on_frame_move_left_hovered();
            }
            animation->setStartValue(QRect(ui->frame_move_right->x(),0,220,440));
            animation->setEndValue(QRect(220,0,220,440));
            sec=float(ui->frame_move_right->x()-220)/220.0*500;
        }else{
            ui->frame_move_right->oren=frame_move::right;
            animation->setStartValue(QRect(ui->frame_move_right->x(),0,220,440));
            animation->setEndValue(QRect(440,0,220,440));
            sec=float(440-ui->frame_move_right->x())/220.0*500;
        }
    }
    animation->setDuration(sec);//最大500毫秒
    animation->start();
}

void page_welcome::on_btn_register_clicked()
{
    //获取用户名、密码
    QString username = ui->lineEdit_register_username->text();
    QString password = ui->lineEdit_register_password->text();
    QString password_again=ui->lineEdit_register_passwordAgain->text();
    if(username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this,"有问题哦！","用户名或者密码为空！",QMessageBox::Yes);
    }
    else if(!(password==password_again)){
        QMessageBox::warning(this,"有问题哦！","两次输入密码不一致！",QMessageBox::Yes);
        ui->lineEdit_register_password->setText("");
        ui->lineEdit_register_passwordAgain->setText("");
    }
    else{
        if(this->isconnection){
            QJsonObject jsonData;
            jsonData["contentType"]="register";
            jsonData["username"]=username;
            jsonData["password"]=password;
            this->username=username;
            QString sendMsg=page_welcome::jsonToString(jsonData);
            server_socket->write(sendMsg.toUtf8());
        }else{
            QMessageBox::warning(this,"提示","网不好呢");
        }
    }
}

void page_welcome::on_btn_login_clicked()
{
    //获取用户名、密码
    QString username=ui->lineEdit_login_username->text();
    QString password=ui->lineEdit_login_password->text();
    if(username.isEmpty()||password.isEmpty()){
        QMessageBox::warning(this,"有问题哦！","用户名或者密码为空！",QMessageBox::Yes);
    }else{
        if(this->isconnection){
            QJsonObject jsonData;
            jsonData["contentType"]="logIn";
            jsonData["username"]=username;
            this->username=username;
            jsonData["password"]=password;
            QString sendMsg=page_welcome::jsonToString(jsonData);
            server_socket->write(sendMsg.toUtf8());
        }else{
            QMessageBox::warning(this,"提示","网不好呢");
        }
    }
}

void page_welcome::on_btn_local_clicked()
{
    emit play_local();
}

void page_welcome::slot_connected(){
    this->isconnection=true;
}

void page_welcome::on_btn_seePassword_clicked()
{
    static bool is_visible=false;
    if(is_visible){
        ui->lineEdit_login_password->setEchoMode(QLineEdit::Password);
        ui->btn_seePassword->setStyleSheet("image: url(:/img/closeEye.png);");
    }else{
        ui->lineEdit_login_password->setEchoMode(QLineEdit::Normal);
        ui->btn_seePassword->setStyleSheet("image: url(:/img/openEye.png);");
    }
    is_visible=!is_visible;
}

void page_welcome::on_btn_link_clicked()
{
    if(!this->isconnection){
        qDebug()<<"尝试连接";;
        static bool have_try=false;
        if(!have_try){
            this->server_socket->connectToHost("127.0.0.1",8080);
            QVBoxLayout*layout_btn=new QVBoxLayout();
            QString orginStyleSheet=ui->btn_link->styleSheet();
            ui->btn_link->setStyleSheet("");
            ui->btn_link->setLayout(layout_btn);
            QLabel*label=new QLabel();
            layout_btn->addWidget(label);
            QMovie *movie = new QMovie(":/gif/link.gif");
            label->setMovie(movie);
            movie->start();
            connect(this->server_socket,&QTcpSocket::connected,[=](){
                if(label!=nullptr&&movie!=nullptr&&layout_btn){
                    qDebug()<<"连接成功";
                    movie->stop();
                    delete movie;
                    delete label;
                    delete layout_btn;
                    ui->btn_link->setStyleSheet(orginStyleSheet);
                    ui->btn_link->show();
                    have_try=false;
                }
            });
            have_try=true;
        }
    }
}

QString page_welcome::getUsername()
{
    return username;
}

void page_welcome::setQTcpSocket(QTcpSocket *server_socket)
{
    this->server_socket=server_socket;
    if(server_socket!=nullptr){
        server_socket->abort();
        server_socket->connectToHost("127.0.0.1",8080);
        connect(server_socket,&QTcpSocket::connected,this,&page_welcome::slot_connected);
        connect(server_socket,&QTcpSocket::readyRead,this,&page_welcome::slot_getData);
    }
}

void page_welcome::setChat_with_server(chat_with_server *chat)
{
    this->chat=chat;
}

void page_welcome::slot_getData(){
    QByteArray getMsg=this->server_socket->readAll();
    chat->read(getMsg.toStdString());
    if(chat->getAnswer()=="登录或注册成功"){
        disconnect(server_socket,&QTcpSocket::readyRead,this,&page_welcome::slot_getData);
        emit signal_switch_page_to_person();
    }else if(chat->getAnswer()=="登录或注册失败"){
        QMessageBox::warning(this,"有问题哦！",chat->jsonData["reason"].toString());
        if(chat->jsonData["reason"].toString()=="用户名已存在"){
            ui->lineEdit_register_username->setText("");
            ui->lineEdit_register_password->setText("");
            ui->lineEdit_register_passwordAgain->setText("");
        }
    }
}


