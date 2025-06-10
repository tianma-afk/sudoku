#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"page_welcome.h"
#include"page_person.h"
#include<QStackedWidget>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QPropertyAnimation>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    page_welcome*obj_page_welcome;
    page_person*obj_page_person;
    QWidget*game_board=0;
    QWidget*game_board_2=0;
    Ui::MainWindow *ui;
    QStackedWidget*stack;
    QString username;
    QTcpSocket*server_socket;
    chat_with_server*chat;
    void changeSettingVisible(const bool isVisible);
    QMediaPlayer *mediaPlayer;
    QAudioOutput*audioOutput;

    bool isDarkMode=false;
    void firstPlay();
private slots:
    void slot_switch_page_to_gameBoard_2();
    void slot_switch_page_to_gameBorad();
    void slot_switch_page_to_person();
    void slot_disconnect();
    void on_btn_music_clicked();
    void on_btn_style_clicked();
    void on_btn_link_clicked();
    void slot_play_local();
    void slot_switch_board_to_welcome();

private://一些样式和文件
    const QString lightStyle=R"(QWidget#backgroundWidget{
                                background-image: url(:/img/background.png);
                                background-repeat:no-repeat;
                                background-position: center;})";
    const QString darkStyle=R"( QPushButton{
                                    background-color:#FFFFFF;}
                                QWidget#backgroundWidget{
                                    background-image: url(:/img/background_night.png);
                                    background-repeat:no-repeat;
                                    background-position: center;})";
    const QUrl Url_BGM=QUrl("qrc:/music/bgm.mp3");
};
#endif // MAINWINDOW_H
