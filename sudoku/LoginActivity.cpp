#include"account.h"

//登录页面类

LoginActivity::LoginActivity(){
	std::string inputID;
	std::string inputPassword;
	Database db;

	do {
		//清空控制台
		system("cls");

		std::cout << "ID:";
		std::cin >> inputID;
		setID(inputID);
		std::cout << "password:";
		std::cin >> inputPassword;
		setPassword(inputPassword);
	} while (!db.isIDPasswordMatched(inputID,inputPassword));
}

void LoginActivity::setID(const std::string& inputID) {
	this->ID = inputID;
}

std::string LoginActivity::getID()const {
	return ID;
}

void LoginActivity::setPassword(const std::string& inputPassword){
	this->password = inputPassword;
}

std::string LoginActivity::getPassword() const{
	return password;
}


