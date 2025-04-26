#include "page_welcome.h"
#include "ui_page_welcome.h"
#include<QPropertyAnimation>
#include<QCoreApplication>
#include<QDebug>
//#include<QWidget>
page_welcome::page_welcome(page *parent)
    : page(parent)
    , ui(new Ui::page_welcome)
{
    ui->setupUi(this);
    ui->frame_move_left->setGeometry(220,0,220,440);
    ui->frame_move_right->setGeometry(440,0,220,440);

    connect(ui->frame_move_left,&frame_move::signal_hover,this,&page_welcome::on_frame_move_left_hovered);
    connect(ui->frame_move_right,&frame_move::signal_hover,this,&page_welcome::on_frame_move_right_hovered);
    connect(ui->btn_register,&QPushButton::clicked,[=](){

    });
    connect(ui->btn_exit,&QPushButton::clicked,[=](){
        QCoreApplication::quit();
    });
}

page_welcome::~page_welcome()
{
    delete ui;
}

void page_welcome::on_frame_move_left_hovered()
{
    //qDebug()<<"进入左边的";
    qDebug()<<ui->frame_move_left->x();
    QPropertyAnimation*animation=new QPropertyAnimation(ui->frame_move_left,"geometry");
    if(ui->frame_move_left->x()<220){
        //如果左滑块在最左边
        //qDebug()<<"在左边";
        if(ui->frame_move_right->x()<440){
            on_frame_move_right_hovered();
        }
        animation->setStartValue(QRect(0,0,220,440));
        animation->setEndValue(QRect(220,0,220,440));
    }else{
        //在中间
        qDebug()<<"在中间";
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
        //qDebug()<<"在右边";
        if(ui->frame_move_left->x()>=220){
            on_frame_move_left_hovered();
        }
        animation->setStartValue(QRect(440,0,220,440));
        animation->setEndValue(QRect(220,0,220,440));
    }else{
        //在中间
        //qDebug()<<"在中间";
        //qDebug()<<ui->frame_move_right->x();
        animation->setStartValue(QRect(220,0,220,440));
        animation->setEndValue(QRect(440,0,220,440));
    }
    animation->setDuration(500);//500毫秒
    animation->start();
}
