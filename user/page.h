#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include<QJsonDocument>
#include<QJsonObject>
class page:public QWidget
{
    Q_OBJECT
public:
    explicit page(QWidget*parent=nullptr);
public:
    static QJsonObject stringToJson(const QString&jsonStr);
    static QString jsonToString(const QJsonObject&jsonObj);
signals:
    void signal_switch_page();
};
#endif // PAGE_H
