#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include<QJsonObject>
#include<QJsonDocument>
class page:public QWidget
{
    Q_OBJECT
public:
    explicit page(QWidget*parent=nullptr);
public:
    static QJsonObject stringToJson(const QString&jsonStr);
    static QString jsonToString(const QJsonObject&jsonObj);
signals:
    void signal_switch_page(const int id);
};
#endif // PAGE_H
