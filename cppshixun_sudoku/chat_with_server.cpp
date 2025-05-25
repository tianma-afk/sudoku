#include "chat_with_server.h"
#include<QMessageBox>
chat_with_server::chat_with_server() {}

void chat_with_server::read(string msg)
{
    this->jsonData=page::stringToJson(QString::fromStdString(msg));
    if(jsonData["answerType"]=="logIn"||jsonData["answerType"]=="register"){
        if(jsonData["isOK"]==true){
            words="登录或注册成功";
        }else{
            words="登录或注册失败";
        }
    }else if(jsonData["answerType"]=="getOldArchive"){
        words="得到存档";
    }else if(jsonData["answerType"]=="saveArchive"){
        if(jsonData["isOk"]==true){
            words="保存成功";
        }else{
            words="保存失败";
        }
    }else if(jsonData["answerType"]=="success"||jsonData["answerType"]=="defeat"){
        if(jsonData["isOK"]==true){
            words="结算成功";
        }else{
            words="结算失败";
        }
    }
}

QString chat_with_server::getAnswer()
{
    return words;
}
