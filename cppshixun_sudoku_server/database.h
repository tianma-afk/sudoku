#ifndef DATABASE_H
#define DATABASE_H
#include"user.h"
#include<vector>
#include<QFile>
using std::vector;

class DataBase
{
public:
    DataBase();
    ~DataBase();
    void loadFromFile();
    void DownLoadInFile();

    void add(const User&user);
    void remove(const QString& username);
    void change(const User& user);
    User search(const QString& username);
    bool isExit(const QString& username);
    void addArchive(const QString&username,const QString&archiveid,const SDKBoardList&list);
    void deleteArchive(const QString&username,const QString&archiveid);
    void changeShore(const QString&username,const QString &archiveid,const bool&change);
    vector<QPair<QString,int>> getAllNameAndShore()const;
    QMap<QString,SDKBoardList> getArchives(const QString&username);
private:
    vector<User>UserArr;
    QString fileloc="./data.bin";
};

#endif // DATABASE_H
