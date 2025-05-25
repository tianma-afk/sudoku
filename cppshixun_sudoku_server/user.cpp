#include "user.h"

User::User(QString username,QString password) {
    this->username=username;
    this->password=password;
    shore=0;
}
User::User(){

}
QString User::getUsername() const
{
    return username;
}

QString User::getPassword() const
{
    return password;
}

int User::getShore() const
{
    return shore;
}


QMap<QString, SDKBoardList> User::getDocuments() const
{
    return documents;
}

void User::setUsername(const QString &newUsername)
{
    username = newUsername;
}

void User::setPassword(const QString &newPassword)
{
    password = newPassword;
}

void User::setShore(const int &shore)
{
    this->shore=shore;
}

void User::setDocuments(const QMap<QString, SDKBoardList> &newDocuments)
{
    documents=newDocuments;
}

void User::addDocument(const QString &archiveid, const SDKBoardList &list)
{
    for(int i=0;i<9;i++){ for(int j=0;j<9;j++){documents[archiveid].data[0][i][j]=list.data[0][i][j];}}
    for(int i=0;i<9;i++){ for(int j=0;j<9;j++){documents[archiveid].data[1][i][j]=list.data[1][i][j];}}
    for(int i=0;i<9;i++){ for(int j=0;j<9;j++){documents[archiveid].data[2][i][j]=list.data[2][i][j];}}
}

void User::deleteDocument(const QString &archiveid)
{
    documents.remove(archiveid);
}

QDataStream&operator<<(QDataStream&out,const User&user){
    out<<user.username;
    out<<user.password;
    out<<user.shore;
    out<<user.documents;
    return out;
}

QDataStream&operator>>(QDataStream&in,User&user){
    in>>user.username;
    in>>user.password;
    in>>user.shore;
    in>>user.documents;
    return in;
}
