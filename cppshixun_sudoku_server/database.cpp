#include "database.h"
#include<QDebug>

DataBase::DataBase() {
    loadFromFile();
}
DataBase::~DataBase(){
    DownLoadInFile();
}
void DataBase::loadFromFile()
{
    QFile file(fileloc);

    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"文件打开失败";
        return;
    }
    QDataStream in(&file);
    User user;
    while(!in.atEnd()){
        in>>user;
        UserArr.push_back(user);
        qDebug()<<user.getUsername()
                <<user.getPassword()
                 <<user.getShore();
        qDebug()<<user.getDocuments().size();
    }
    file.close();
}

void DataBase::DownLoadInFile()
{
    QFile file(fileloc);

    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"文件打开失败";
        return;
    }
    QDataStream out(&file);
    for(const User user:UserArr){
        out<<user;
    }
    file.close();
}

void DataBase::add(const User &user)
{
    UserArr.emplace_back(user);
}

void DataBase::remove(const QString &username)
{
    if(isExit(username)){
        vector<User>::iterator it=UserArr.begin();
        for(int i=0;i<UserArr.size();i++){
            if(UserArr.at(i).getUsername()==username){
                it+=i;
                break;
            }
        }
        UserArr.erase(it);
    }else{
        qDebug()<<"不存在这个人";
    }
}

void DataBase::change(const User &user)
{

}

User DataBase::search(const QString &username)
{
    for(User&user:UserArr){
        if(user.getUsername()==username){
            return user;
        }
    }
}

bool DataBase::isExit(const QString &username)
{
    for(const User&user:UserArr){
        if(user.getUsername()==username){
            return true;
        }
    }
    return false;
}

void DataBase::addArchive(const QString &username, const QString &archiveid, const SDKBoardList &list)
{
    for(User&user:UserArr){
        if(user.getUsername()==username){
            user.addDocument(archiveid,list);
            return;
        }
    }
}

void DataBase::deleteArchive(const QString &username, const QString &archiveid)
{
    for(User&user:UserArr){
        if(user.getUsername()==username){
            user.deleteDocument(archiveid);
            return;
        }
    }
}

void DataBase::changeShore(const QString &username,const QString &archiveid ,const bool &change)
{
    for(User&user:UserArr){
        if(user.getUsername()==username){
            if(change){
                SDKBoardList list=user.getDocuments().find(archiveid).value();
                int space=0;
                for(int i=0;i<9;i++){for(int j=0;j<9;j++){if(list.data[0][i][j]==0)space++;}}
                user.setShore(user.getShore()+81-space);
            }
            return;
        }
    }
}

vector<QPair<QString,int>> DataBase::getAllNameAndShore() const
{
    vector<QPair<QString,int>>shoreList;
    for(const User&user:UserArr){
        QPair<QString,int>pair(user.getUsername(),user.getShore());
        shoreList.push_back(pair);
    }
    return shoreList;
}

QMap<QString, SDKBoardList> DataBase::getArchives(const QString &username)
{
    for(User&user:UserArr){
        if(user.getUsername()==username){
            return user.getDocuments();
        }
    }
}
