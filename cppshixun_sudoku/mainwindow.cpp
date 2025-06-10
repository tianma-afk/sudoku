#include "mainwindow.h"
#include<QLabel>
#include "ui_mainwindow.h"
#include<QDebug>
#include"gameboard.h"
#include"gameboard_2.h"
#include<QVBoxLayout>
#include<QPainter>
#include<QMovie>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    changeSettingVisible(false);
    statusBar()->setStyleSheet("QStatusBar { background-color: #0FFD9D; }");

    QScreen*screen=QApplication::primaryScreen();
    QRect screenGeometry=screen->availableGeometry();
    ui->backgroundWidget->setGeometry(screenGeometry);

    ui->BKVlayout->setContentsMargins(0, 0, 0, 0);
    ui->BKVlayout->setAlignment(Qt::AlignCenter);


    this->server_socket=new QTcpSocket();
    this->chat=new chat_with_server();
    this->mediaPlayer=new QMediaPlayer(this);
    this->audioOutput=new QAudioOutput(this);
    connect(server_socket,&QTcpSocket::disconnected,this,&MainWindow::slot_disconnect);

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);


    obj_page_welcome=new page_welcome;
    obj_page_person=new page_person;
    obj_page_welcome->setQTcpSocket(this->server_socket);
    obj_page_welcome->setChat_with_server(this->chat);

    stack=new QStackedWidget(this);
    ui->BKVlayout->addWidget(stack);
    stack->addWidget(obj_page_welcome);
    stack->addWidget(obj_page_person);
    connect(obj_page_welcome,&page_welcome::signal_switch_page_to_person,this,&MainWindow::slot_switch_page_to_person);
    connect(obj_page_person,&page_person::signal_switch_page_to_GameBoard,this,&MainWindow::slot_switch_page_to_gameBorad);
    connect(obj_page_person,&page_person::signal_switch_page_to_GameBoard_2,this,&MainWindow::slot_switch_page_to_gameBoard_2);
    connect(obj_page_welcome,&page_welcome::play_local,this,&MainWindow::slot_play_local);
}

MainWindow::~MainWindow()
{

    delete ui;
    delete chat;
    server_socket->deleteLater();
    delete obj_page_welcome;
    delete obj_page_person;
}

void MainWindow::changeSettingVisible(const bool isVisible)
{
    if(isVisible){
        for(int i=0;i<ui->SettingLayout->count();i++){
            QWidget*widget=ui->SettingLayout->itemAt(i)->widget();
            if(widget){
                widget->setVisible(true);
            }
        }
    }else{
        for(int i=0;i<ui->SettingLayout->count();i++){
            QWidget*widget=ui->SettingLayout->itemAt(i)->widget();
            if(widget){
                widget->setVisible(false);
            }
        }
    }
}

void MainWindow::slot_switch_page_to_gameBorad()
{
    if(game_board==nullptr){
        game_board=new GameBoard();
    }
    this->username=obj_page_welcome->getUsername();
    ((GameBoard*)game_board)->setUsername(username);

    ((GameBoard*)game_board)->setQTcpSocket(this->server_socket);
    ((GameBoard*)game_board)->setChat_with_server(this->chat);

    game_board->setAttribute(Qt::WA_TranslucentBackground);
    if(stack->count()<3){
        stack->addWidget(game_board);
    }
    stack->setCurrentIndex(2);
    connect((GameBoard*)game_board,&GameBoard::signal_switch_board_to_person,this,&MainWindow::slot_switch_page_to_person);
}

void MainWindow::slot_switch_page_to_person()
{
    this->username=obj_page_welcome->getUsername();
    obj_page_person->setUsername(this->username);
    obj_page_person->setQTcpSocket(this->server_socket);
    obj_page_person->setChat_with_server(this->chat);

    stack->setCurrentIndex(1);

    this->setStyleSheet("QWidget#backgroundWidget{"
                        "background-image: url(:/img/background.png);"
                        "background-repeat:no-repeat;"
                        "background-position: center;"
                        "}");
    this->setWindowFlag(Qt::FramelessWindowHint,false);
    this->setAttribute(Qt::WA_TranslucentBackground,false);
    changeSettingVisible(true);
    this->showMaximized();
    statusBar()->setStyleSheet("QStatusBar { background-color: #0FFD9D; }");
    statusBar()->showMessage("无上尊贵的 "+this->username+ "欢迎回家!");
    firstPlay();
}

void MainWindow::firstPlay(){
    audioOutput->setVolume(0.5);
    this->mediaPlayer->setAudioOutput(this->audioOutput);
    this->mediaPlayer->setSource(Url_BGM);
    this->mediaPlayer->setLoops(QMediaPlayer::Infinite);
    this->mediaPlayer->play();
}

void MainWindow::slot_switch_page_to_gameBoard_2()
{
    if(stack->count()<3){
        if(game_board==nullptr){
            game_board=new GameBoard();
        }
        stack->addWidget(game_board);
    }
    if(game_board_2==nullptr){
        game_board_2=new GameBoard_2();    
        stack->addWidget(game_board_2);
    }
    ((GameBoard_2*)game_board_2)->set_is_local(false);
    stack->setCurrentIndex(3);
    connect((GameBoard_2*)game_board_2,&GameBoard_2::signal_switch_board_to_person,this,&MainWindow::slot_switch_page_to_person);
}

void MainWindow::slot_disconnect()
{
    statusBar()->showMessage("啊哦。。。网络断开了呢！");
    QMessageBox::warning(this,"提示","连接断开了。。。");
    server_socket->close();
    this->obj_page_welcome->isconnection=false;
}

void MainWindow::on_btn_music_clicked()
{
        if(mediaPlayer->playbackState()==QMediaPlayer::PlayingState){
            this->mediaPlayer->pause();
            statusBar()->showMessage("音乐暂停",3000);
            ui->btn_music->setStyleSheet("image: url(:/img/music_pause.png);");
        }else{
            this->mediaPlayer->play();
            statusBar()->showMessage("音乐播放",3000);
            ui->btn_music->setStyleSheet("image: url(:/img/music_play.png);");
        }
}

void MainWindow::on_btn_style_clicked()
{
    if(this->isDarkMode){
        this->setStyleSheet(this->lightStyle);
        statusBar()->showMessage("白天模式",3000);
        ui->btn_style->setStyleSheet("image: url(:/img/day.png);");
    }else{
        this->setStyleSheet(this->darkStyle);
        statusBar()->showMessage("夜晚模式",3000);
        ui->btn_style->setStyleSheet("image: url(:/img/night.png);");
    }
    isDarkMode=!isDarkMode;
}

void MainWindow::on_btn_link_clicked()
{
    if(this->server_socket->state()==QAbstractSocket::ConnectedState){
        statusBar()->showMessage("连接正常",3000);
    }else{
        statusBar()->showMessage("尝试连接网络",3000);
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
                    statusBar()->showMessage("连接成功",3000);
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

void MainWindow::slot_play_local()
{
    this->setStyleSheet("QWidget#backgroundWidget{"
                        "background-image: url(:/img/background.png);"
                        "background-repeat:no-repeat;"
                        "background-position: center;"
                        "}");
    this->setWindowFlag(Qt::FramelessWindowHint,false);
    this->setAttribute(Qt::WA_TranslucentBackground,false);
    changeSettingVisible(true);
    this->showMaximized();
    if(game_board_2==nullptr){
        game_board_2=new GameBoard_2();
        stack->addWidget(game_board_2);
    }
    ((GameBoard_2*)game_board_2)->set_is_local(true);
    stack->setCurrentIndex(2);
    connect((GameBoard_2*)game_board_2,&GameBoard_2::signal_switch_board_to_welcome,this,&MainWindow::slot_switch_board_to_welcome);
}

void MainWindow::slot_switch_board_to_welcome()
{
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    changeSettingVisible(false);
    this->showMaximized();
    stack->setCurrentIndex(0);
    stack->removeWidget(game_board_2);
    delete (GameBoard_2*)game_board_2;
    game_board_2=nullptr;
}


