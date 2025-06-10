#include "page_person.h"
#include "ui_page_person.h"
#include<QPixmap>
page_person::page_person(page *parent)
    : page(parent)
    , ui(new Ui::page_person)
{
    ui->setupUi(this);
}

page_person::~page_person()
{
    delete ui;
}

void page_person::setUsername(const QString &username)
{
    this->username=username;

    ui->label_person->setText(this->username);
}

void page_person::setChat_with_server(chat_with_server *chat)
{
    this->chat=chat;
}

void page_person::setQTcpSocket(QTcpSocket *server_socket)
{
    this->server_socket=server_socket;
    if(server_socket!=nullptr){
        this->isconnection=true;
        connect(server_socket,&QTcpSocket::connected,this,&page_person::slot_connected);
        connect(server_socket,&QTcpSocket::readyRead,this,&page_person::slot_getData);
    }
}

void page_person::init_rank()
{
    ui->rankList->clear();
    int rankSize=chat->jsonData["ranksize"].toInt();
    ui->rankList->setIconSize(QSize(50,50));
    for(int i=1;i<=rankSize;i++){
        RankItem* item=new RankItem();
        QString name=chat->jsonData["rank"+QString::number(i)+"name"].toString();
        int shore=chat->jsonData["rank"+QString::number(i)+"shore"].toInt();
        item->setText(name+"\t"+QString::number(shore));
        ui->rankList->addItem(item);
    }
}

void page_person::on_btn_game_clicked()
{
    qDebug()<<"game被点击";
    disconnect(server_socket,&QTcpSocket::readyRead,this,&page_person::slot_getData);
    this->isconnection=false;
    emit signal_switch_page_to_GameBoard();
}

void page_person::on_btn_game_2_clicked()
{
    static bool have_say=false;
    if(!have_say){
        QMessageBox::information(this,"娱乐模式","娱乐模式没有积分哦！");
        have_say=true;
    }
    emit signal_switch_page_to_GameBoard_2();
}

void page_person::slot_getData()
{
    QByteArray getMsg=this->server_socket->readAll();
    chat->read(getMsg.toStdString());
    if(chat->getAnswer()=="排名获取成功"){
        init_rank();
    }
}

void page_person::slot_connected()
{
    this->isconnection=true;
}

void page_person::slot_disconnect()
{
    this->isconnection=false;
}

void page_person::on_btn_rank_clicked()
{
    if(this->isconnection){
        QJsonObject jsonData;
        jsonData["contentType"]="checkRank";
        QString sendMsg=page::jsonToString(jsonData);
        server_socket->write(sendMsg.toUtf8());
    }else{
        QMessageBox::warning(this,"提示","网不好呢");
    }
}
