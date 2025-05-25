#include "page.h"

page::page(QWidget*parent):QWidget(parent) {

}
QJsonObject page::stringToJson(const QString&jsonStr){
    QJsonDocument doc=QJsonDocument::fromJson(jsonStr.toUtf8());
    if(doc.isObject()){
        return doc.object();
    }
    return QJsonObject();
}

QString page::jsonToString(const QJsonObject&jsonObj){
    QJsonDocument doc(jsonObj);
    return QString(doc.toJson(QJsonDocument::Compact));
}
