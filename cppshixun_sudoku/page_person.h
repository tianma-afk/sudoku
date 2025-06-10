#ifndef PAGE_PERSON_H
#define PAGE_PERSON_H

#include <QWidget>
#include"page.h"
#include"chat_with_server.h"
#include<QMessageBox>
#include"rankitem.h"
namespace Ui {
class page_person;
}

class page_person : public page
{
    Q_OBJECT

public:
    bool isconnection=false;
    explicit page_person(page *parent = nullptr);
    ~page_person();
    void setUsername(const QString&username);
    void setChat_with_server(chat_with_server*chat);
    void setQTcpSocket(QTcpSocket*server_socket);
private:
    Ui::page_person *ui;
    QString username;
    chat_with_server* chat;
    QTcpSocket*server_socket;
    void init_rank();
private slots:
    void on_btn_game_clicked();
    void on_btn_game_2_clicked();
    void slot_getData();
    void slot_connected();
    void slot_disconnect();
    void on_btn_rank_clicked();
};

#endif // PAGE_PERSON_H
