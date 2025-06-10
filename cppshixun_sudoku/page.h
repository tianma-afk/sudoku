#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include<QJsonObject>
#include<QJsonDocument>

#include<QTcpSocket>
class page:public QWidget
{
    Q_OBJECT
public:
    explicit page(QWidget*parent=nullptr);
    static QJsonObject stringToJson(const QString&jsonStr);
    static QString jsonToString(const QJsonObject&jsonObj);

signals:
    void signal_switch_page_to_GameBoard();
    void signal_switch_page_to_person();
    void signal_switch_page_to_GameBoard_2();
};
#endif // PAGE_H
