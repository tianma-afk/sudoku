#include "gamemanager.h"

QJsonObject stringToJson(const QString&jsonStr){
    QJsonDocument doc=QJsonDocument::fromJson(jsonStr.toUtf8());
    if(doc.isObject()){
        return doc.object();
    }
    return QJsonObject();
}

QString jsonToString(const QJsonObject&jsonObj){
    QJsonDocument doc(jsonObj);
    return QString(doc.toJson(QJsonDocument::Compact));
}

GameManager::GameManager() {}

void GameManager::readData(QString data)
{
    QJsonObject jsonData=stringToJson(data);
    QJsonObject jsonRet;
    if(jsonData["contentType"]=="register"){//注册
        jsonRet=registerAsk(jsonData);
    }else if(jsonData["contentType"]=="logIn"){//登录
        jsonRet=logInAsk(jsonData);
    }else if(jsonData["contentType"]=="checkRank"){//检查排名
        jsonRet=checkRankAsk(jsonData);
    }else if(jsonData["contentType"]=="newGame"){//新游戏
        jsonRet=newGameAsk(jsonData);
    }else if(jsonData["contentType"]=="oldGame"){//玩存档游戏
        jsonRet=oldGameAsk(jsonData);
    }else{
        jsonRet=exceptionAsk();
    }
    words=jsonToString(jsonRet);
}

QString GameManager::returnData()
{
    return words;
}

QJsonObject GameManager::registerAsk(QJsonObject data)
{
    QJsonObject ret;
    if(DB.isExit(data["username"].toString().toStdString())){
        ret["isOK"]=false;
        ret["reason"]="用户名已存在";
    }else{
        ret["isOK"]=true;
        User user(data["username"].toString().toStdString(),data["password"].toString().toStdString());
        DB.add(user);
    }
    return ret;
}

QJsonObject GameManager::logInAsk(QJsonObject data)
{
    QJsonObject ret;
    return ret;
}

QJsonObject GameManager::checkRankAsk(QJsonObject data)
{
    QJsonObject ret;
    return ret;
}

QJsonObject GameManager::newGameAsk(QJsonObject data)
{
    QJsonObject ret;
    return ret;
}

QJsonObject GameManager::oldGameAsk(QJsonObject data)
{
    QJsonObject ret;
    return ret;
}

QJsonObject GameManager::exceptionAsk()
{
    QJsonObject ret;
    return ret;
}



