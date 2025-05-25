#ifndef USER_H
#define USER_H

#include<QDataStream>
#include<QString>
#include<QMap>
#include"sdkboardlist.h"


class User
{
public:
    User(QString username,QString password);
    User();
    QString getUsername() const;
    QString getPassword() const;
    int getShore()const;
    QMap<QString, SDKBoardList> getDocuments() const;

    void setUsername(const QString &newUsername);
    void setPassword(const QString &newPassword);
    void setShore(const int &shore);
    void setDocuments(const QMap<QString, SDKBoardList> &newDocuments);
    void addDocument(const QString&archiveid,const SDKBoardList&list);
    void deleteDocument(const QString&archiveid);
    friend QDataStream&operator<<(QDataStream&out,const User&user);
    friend QDataStream&operator>>(QDataStream&in,User&user);
private:
    QString username;
    QString password;
    int shore;
    QMap<QString,SDKBoardList>documents;
};

#endif // USER_H
