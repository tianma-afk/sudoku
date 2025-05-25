#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include<QString>
#include<QJsonDocument>
#include<QJsonObject>
#include"database.h"
class GameManager
{
public:
    GameManager();
    void readData(QString data);
    QString returnData();
private:
    DataBase DB;
    QString words;
    QJsonObject registerAsk(QJsonObject data);
    QJsonObject logInAsk(QJsonObject data);
    QJsonObject checkRankAsk(QJsonObject data);
    QJsonObject saveArchiveAsk(QJsonObject data);
    QJsonObject getOldArchiveAsk(QJsonObject data);
    QJsonObject successAsk(QJsonObject data);
    QJsonObject defeatAsk(QJsonObject data);
    QJsonObject exceptionAsk();

};




#endif // GAMEMANAGER_H
