#include "user.h"

User::User(string username,string password) {
    this->username=username;
    this->password=password;
}
User::User(){

}
string User::getUsername() const
{
    return username;
}

string User::getPassword() const
{
    return password;
}


map<string, SDKBoard> User::getDocuments() const
{
    return documents;
}

void User::setUsername(const string &newUsername)
{
    username = newUsername;
}

void User::setPassword(const string &newPassword)
{
    password = newPassword;
}

void User::setDocuments(const map<string, SDKBoard> &newDocuments)
{
    documents = newDocuments;
}
