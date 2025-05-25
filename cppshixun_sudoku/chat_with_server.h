#ifndef CHAT_WITH_SERVER_H
#define CHAT_WITH_SERVER_H
#include"page.h"
#include<string>
using std::string;
class chat_with_server
{
public:
    chat_with_server();
    void read(string msg);
    QString getAnswer();
    QJsonObject jsonData;
private:
    QString words;

};

#endif // CHAT_WITH_SERVER_H
