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
    ifstream in;
    in.open(fileloc,std::ios_base::in|std::ios_base::binary);
    if(in.is_open()){
        //读取用户数组大小
        size_t user_size=0;
        in.read(reinterpret_cast<char*>(&user_size),sizeof(user_size));
        UserArr.resize(user_size);
        qDebug()<<user_size;
        for(User&user:UserArr){
            string username;
            string password;
            //读取用户名大小
            size_t username_size=0;
            in.read(reinterpret_cast<char*>(&username_size),sizeof(username_size));
            qDebug()<<username_size;
            //读取用户名内容
            username.resize(username_size);
            in.read(&username[0],username_size);
            qDebug()<<QString::fromStdString(username);
            //读取密码大小
            size_t password_size=0;
            in.read(reinterpret_cast<char*>(&password_size),sizeof(password_size));
            //读取密码内容
            password.resize(password_size);
            in.read(&password[0],password_size);
            qDebug()<<QString::fromStdString(password);
            //读取存档数
            size_t document_size=0;
            in.read(reinterpret_cast<char*>(&document_size),sizeof(document_size));
            qDebug()<<document_size;
            map<string,SDKBoard>documents;
            //读取存档
            for(size_t i=0;i<document_size;i++){
                string docname;
                //获取存档名大小
                size_t docname_size=0;
                SDKBoard board;
                in.read(reinterpret_cast<char*>(&docname_size),sizeof(docname_size));
                //获取存档名内容
                in.read(reinterpret_cast<char*>(&docname),docname_size);
                //获取数独板
                in.read(reinterpret_cast<char*>(&board),81);
                for(int a=0;a<9;a++){
                    for(int b=0;b<9;b++){
                        documents[docname][a][b]=board[a][b];
                    }
                }
            }
            user.setUsername(username);
            user.setPassword(password);
            user.setDocuments(documents);
        }
    }else{
        ofstream out(fileloc);
        out.close();
    }
}

void DataBase::DownLoadInFile()
{
    ofstream out(fileloc,std::ios_base::out|std::ios_base::binary);
    if(out.is_open()){
        //写入用户数组大小
        size_t user_size=UserArr.size();
        out.write(reinterpret_cast<const char*>(&user_size),sizeof(user_size));
        for(const User&user:UserArr){
            //写入用户名大小
            size_t username_size=user.getUsername().size();
            out.write(reinterpret_cast<const char*>(&username_size),sizeof(username_size));
            //写入用户名内容
            out.write(user.getUsername().c_str(),username_size);
            //写入密码大小
            size_t password_size=user.getPassword().size();
            out.write(reinterpret_cast<const char*>(&password_size),sizeof(password_size));
            //写入密码内容
            out.write(user.getPassword().c_str(),password_size);
            //写入存档数
            size_t document_size=user.getDocuments().size();
            out.write(reinterpret_cast<const char*>(&document_size),sizeof(document_size));
            //写入存档
            for(const pair doc:user.getDocuments()){
                //写入存档名大小
                size_t docname_size=doc.first.size();
                out.write(reinterpret_cast<const char*>(&docname_size),sizeof(docname_size));
                //写入文档名内容
                out.write(reinterpret_cast<const char*>(&doc.first),docname_size);
                //写入数独板
                out.write(reinterpret_cast<const char*>(&doc.second),81);
            }
        }
    }else{
        qDebug()<<"写入文档失败";
    }
}

void DataBase::add(const User &user)
{
    UserArr.emplace_back(user);
}

void DataBase::remove(const string &username)
{

}

void DataBase::change(const User &user)
{

}

User DataBase::search(const string &username)
{
    for(User&user:UserArr){
        if(user.getUsername()==username){
            return user;
        }
    }
}

bool DataBase::isExit(const string &username)
{
    for(const User&user:UserArr){
        if(user.getUsername()==username){
            return true;
        }
    }
    return false;
}
