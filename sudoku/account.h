#include<iostream>
#include<string>
#include"database.h"

#pragma once


//��¼ҳ����
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


//ע��ҳ����
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


//�û���
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

//�û�������
class UserManager {
public:
	UserManager();
	//���User�Ƿ������ݿ��д��ڣ������ڷ���flase�����ڷ���true
	bool checkUser(User&user);
	void registerUser(User& user);
	~UserManager();
private:
	Database database;
};