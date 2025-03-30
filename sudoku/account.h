#include<iostream>
#include<string>
#include"database.h"

#pragma once


//登录页面类
class LoginActivity {
public:
	LoginActivity();
	void setID(const std::string& inputID);
	std::string getID()const;
	void setPassword(const std::string& inputPassword);
	std::string getPassword()const;

private:
	std::string ID;
	std::string password;
};


//注册页面类
class RegisterActivity {
public:
	RegisterActivity();
	void setID(const std::string& inputID);
	std::string getID()const;
	void setPassword(const std::string& inputPassword);
	std::string getPassword()const;

private:
	std::string ID;
	std::string password;
};


//用户类
class User {
public:
	User();
	void setID(const std::string& inputID);
	std::string getID()const;
	void setPassword(const std::string& inputPassword);
	std::string getPassword()const;
private:
	std::string ID;
	std::string password;
};

//用户管理类
class UserManager {
public:
	UserManager();
	//检查User是否在数据库中存在，不存在返回flase，存在返回true
	bool checkUser(User&user);
	void registerUser(User& user);
	~UserManager();
private:
	Database database;
};