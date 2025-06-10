#ifndef PAGE_WELCOME_H
#define PAGE_WELCOME_H
#include"frame_move.h"
#include <QWidget>
#include"page.h"
#include"chat_with_server.h"
namespace Ui {
class page_welcome;
}

class page_welcome : public page
{
    Q_OBJECT

public:
    bool isconnection=false;
    explicit page_welcome(page*parent = nullptr);
    ~page_welcome();
    QString getUsername();
    void setChat_with_server(chat_with_server*chat);
    void setQTcpSocket(QTcpSocket*server_socket);
public:
signals:
    void play_local();
private:
    Ui::page_welcome *ui;
private slots:
    void on_frame_move_left_hovered();
    void on_frame_move_right_hovered();
    void on_btn_register_clicked();
    void on_btn_login_clicked();
    void on_btn_local_clicked();

    void on_btn_seePassword_clicked();
    void on_btn_link_clicked();
    void slot_getData();
    void slot_connected();
private:
    chat_with_server* chat;
QTcpSocket*server_socket;

    QString username;
};

#endif // PAGE_WELCOME_H
