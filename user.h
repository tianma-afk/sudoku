#ifndef USER_H
#define USER_H
#include<string>
#include<map>
#include<vector>
using std::string;
using std::map;
using std::vector;
using SDKBoard=char[9][9];
class User
{
public:
    User(string username,string password);
    User();
    string getUsername() const;
    string getPassword() const;
    map<string, SDKBoard> getDocuments() const;

    void setUsername(const string &newUsername);
    void setPassword(const string &newPassword);
    void setDocuments(const map<string, SDKBoard> &newDocuments);

private:
    string username;
    string password;
    map<string,SDKBoard>documents;
};

#endif // USER_H
