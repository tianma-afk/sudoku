#ifndef DATABASE_H
#define DATABASE_H
#include"user.h"
#include<vector>
#include<string>
#include<fstream>
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;
using std::pair;
class DataBase
{
public:
    DataBase();
    ~DataBase();
    void loadFromFile();
    void DownLoadInFile();

    void add(const User&user);
    void remove(const string& username);
    void change(const User& user);
    User search(const string& username);
    bool isExit(const string& username);
private:
    vector<User>UserArr;
    string fileloc="./data.bin";
};

#endif // DATABASE_H
