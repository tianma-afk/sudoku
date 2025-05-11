#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QString>
#include"gamemanager.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
private slots:
    void on_btn_start_clicked();
    void on_btn_stop_clicked();
private slots:
    void slot_newConnetion();
    void slot_getData();
    void slot_sendData();
    void slot_disconnect();
private:
    QTcpServer *TCP_server;
    QTcpSocket *client_socket;
    QMap<QString,QTcpSocket*> s_map;
    GameManager gameManager;
};
#endif // WIDGET_H
