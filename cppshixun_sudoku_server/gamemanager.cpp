#include "gamemanager.h"
#include<QString>
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
    }else if(jsonData["contentType"]=="saveArchive"){//保存存档
        jsonRet=saveArchiveAsk(jsonData);
    }else if(jsonData["contentType"]=="getOldArchive"){//获取过去的存档
        jsonRet=getOldArchiveAsk(jsonData);
    }else if(jsonData["contentType"]=="success"){//获胜结算
        jsonRet=successAsk(jsonData);
    }else if(jsonData["contentType"]=="defeat"){//失败结算
        jsonRet=defeatAsk(jsonData);
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
    ret["answerType"]="register";
    if(DB.isExit(data["username"].toString())){
        ret["isOK"]=false;
        ret["reason"]="用户名已存在";
    }else{
        ret["isOK"]=true;
        User user(data["username"].toString(),data["password"].toString());
        DB.add(user);
    }
    return ret;
}

QJsonObject GameManager::logInAsk(QJsonObject data)
{
    QJsonObject ret;
    ret["answerType"]="logIn";
    if(DB.isExit(data["username"].toString())){
        User user=DB.search(data["username"].toString());
        if(user.getPassword()==data["password"].toString()){
            ret["isOK"]=true;
        }else{
            ret["isOK"]=false;
            ret["reason"]="账号或密码错误";
        }
    }else{
        ret["isOK"]=false;
        ret["reason"]="账号或密码错误";
    }
    return ret;
}

QJsonObject GameManager::checkRankAsk(QJsonObject data)
{

    QJsonObject ret;
    ret["answerType"]="checkRank";
    vector<QPair<QString,int>>rank=DB.getAllNameAndShore();
    for(size_t i=0;i<rank.size();i++){
        for(size_t j=i;j<rank.size();j++){
            if(rank.at(i).second<rank.at(j).second){
                QPair<QString,int> tmp=rank.at(i);
                rank.at(i)=rank.at(j);
                rank.at(j)=tmp;
            }
        }
    }
    int rankSize=rank.size();
    ret["ranksize"]=rankSize;
    for(int i=1;i<=rankSize;i++){
        ret["rank"+QString::number(i)+"name"]=rank.at(i-1).first;
        ret["rank"+QString::number(i)+"shore"]=rank.at(i-1).second;
    }
    return ret;
}

QJsonObject GameManager::saveArchiveAsk(QJsonObject data)
{
    QJsonObject ret;
    ret["answerType"]="saveArchive";
    SDKBoardList list;
    for(int i=0;i<9;i++){for(int j=0;j<9;j++){list.data[0][i][j]=data["beginBoard"].toString().at(i*9+j).digitValue();}}
    for(int i=0;i<9;i++){for(int j=0;j<9;j++){list.data[1][i][j]=data["nowBoard"].toString().at(i*9+j).digitValue();}}
    for(int i=0;i<9;i++){for(int j=0;j<9;j++){list.data[2][i][j]=data["answer"].toString().at(i*9+j).digitValue();}}
    DB.addArchive(data["username"].toString(),data["archiveid"].toString(),list);
    ret["isOk"]=true;
    return ret;
}

QJsonObject GameManager::getOldArchiveAsk(QJsonObject data)
{
    QJsonObject ret;
    ret["answerType"]="getOldArchive";
    QMap<QString,SDKBoardList>documents=DB.getArchives(data["username"].toString());
    ret["archiveNum"]=documents.size();
    QMap<QString,SDKBoardList>::const_iterator it;
    int num=0;
    for(it=documents.constBegin();it!=documents.constEnd();it++){
        ret["archiveid"+QString::number(num)]=it.key();
        QString beginBoard;
        QString nowBoard;
        QString answer;
        for(int i=0;i<9;i++){for(int j=0;j<9;j++){beginBoard+=QString::number(it.value().data[0][i][j]);}}
        for(int i=0;i<9;i++){for(int j=0;j<9;j++){nowBoard+=QString::number(it.value().data[1][i][j]);}}
        for(int i=0;i<9;i++){for(int j=0;j<9;j++){answer+=QString::number(it.value().data[2][i][j]);}}
        ret["beginBoard"+QString::number(num)]=beginBoard;
        ret["nowBoard"+QString::number(num)]=nowBoard;
        ret["answer"+QString::number(num)]=answer;
        num++;
    }
    return ret;
}

QJsonObject GameManager::successAsk(QJsonObject data)
{
    QJsonObject ret;
    ret["answerType"]="success";
    DB.changeShore(data["username"].toString(),data["archiveid"].toString(),true);
    DB.deleteArchive(data["username"].toString(),data["archiveid"].toString());
    ret["isOK"]=true;
    return ret;
}

QJsonObject GameManager::defeatAsk(QJsonObject data)
{
    QJsonObject ret;
    ret["answerType"]="defeat";
    DB.changeShore(data["username"].toString(),data["archiveid"].toString(),false);
    DB.deleteArchive(data["username"].toString(),data["archiveid"].toString());
    ret["isOK"]=true;
    return ret;
}

QJsonObject GameManager::exceptionAsk()
{
    QJsonObject ret;
    return ret;
}



