#include"account.h"

//ע��ҳ����

RegisterActivity::RegisterActivity() {
	std::string inputID;
	std::string inputPassword;
	Database db;

		//��տ���̨
		system("cls");
		do {
			std::cout << "ID:";
			std::cin >> inputID;
			setID(inputID);
			std::cout << "password:";
			std::cin >> inputPassword;
			setPassword(inputPassword);
		}while(!db.addInDatabase(ID, password));
}

void RegisterActivity::setID(const std::string& inputID) {
	this->ID = inputID;
}

std::string RegisterActivity::getID()const {
	return ID;
}

void RegisterActivity::setPassword(const std::string& inputPassword) {
	this->password = inputPassword;
}

std::string RegisterActivity::getPassword() const {
	return password;
}
