#include"database.h"

//���ݿ�Դ�ļ�
//���ݿ��ʼ��
Database::Database() {
	loadAccountsFromFile();
}
 Database::Database(const std::string&inputID){
	loadArchivesFromFile(inputID);
}

//У��ID�������Ƿ�ƥ��
bool Database::isIDPasswordMatched(const std::string&inputID,const std::string&inputPassword){
	for (const auto& account : accounts) {
		if (account.first == inputID) {
			if (account.second == inputPassword) {
				return true;
			}
			return false;
		}
	}
	return false;
}

//�����ݿ��в���ID�Ƿ����
bool Database::searchInDatabase(const std::string& inputID)
{
	for (const auto& account : accounts) {
		if (account.first == inputID) {
				return true;
		}
	}
	return false;
}

//����˺Ų��������м�����û�
bool Database::addInDatabase(const std::string& inputID, const std::string& inputPassword){
	if (searchInDatabase(inputID)) {
		std::cout << "Attention:�û�ID�Ѵ���" << std::endl;
		return false;
	}
	else {
		accounts[inputID] = inputPassword;
		loadRankFromFile();
		rank[inputID] = 0;
		saveAccountsToFile();
		return true;
	}
}



//�����˺ŵ��ļ�
void Database::saveAccountsToFile() {
	std::ofstream file("accounts.txt");
	if (file.is_open()) {
		for (const auto& account : accounts) {
			file << account.first<< " " << account.second<<std::endl;
		}
		file.close();
	}
	else {
		std::cerr << "Error:���˺ű��浽�ļ�ʧ��" << std::endl;
	}
}

//�����˺��ļ�
void Database::loadAccountsFromFile() {
	std::ifstream file("accounts.txt");
	if (file.is_open()) {
		std::string ID, password;
		while (file >> ID >> password) {
			accounts[ID] = password;
		}
	}
	else {
		std::cerr << "Error:�����˺��ļ�ʧ��" << std::endl;
	}
}

//���������浵���ļ�
void Database::saveArchivesToFile(const std::string& ID){
	std::ofstream file(ID + ".txt");
	if (file.is_open()) {
		const auto& userArchives = gameArchives[ID];
		for (const auto&archive:userArchives) {
			file << archive.first << std::endl;
			for (const auto& row : archive.second) {
				for (const int& num : row) {
					file << num<<" ";
				}
				file << std::endl;
			}
		}
		file.close();
	}
	else {
		std::cerr << "Error:��" << ID << "�Ĵ浵���浽�ļ�ʧ��" << std::endl;
	}	
}

//���������ļ��м��س���
void Database::loadRankFromFile()
{
	std::ifstream file("rank.txt");
	if (file.is_open()) {
		std::string ID,int grade;
		while (file >> ID >> grade) {
			rank[ID] = grade;
		}
	}
	else {
		std::cerr << "Error:���������ļ�ʧ��" << std::endl;
	}
}

//�����������ļ���
void Database::saveRankToFile() {
	std::ofstream file("rank.txt");
	if (file.is_open()) {
		for (const auto& perRank : rank) {
			file << perRank.first << " " << perRank.second << std::endl;
		}
		file.close();
	}
	else {
		std::cerr << "Error:���������浽�ļ�ʧ��" << std::endl;
	}
}



//�����ļ��е������浵
void Database::loadArchivesFromFile(const std::string& ID) {
	std::ifstream file(ID + ".txt");
	if (file.is_open()) {
		int archiveID;
		while (file >> archiveID) {
			sudokuBoard board(SUDOKU_SIZE, std::vector<int>(SUDOKU_SIZE)); // ��ʼ����άvector
			for (int i = 0; i < SUDOKU_SIZE; ++i) {
				for (int j = 0; j < SUDOKU_SIZE; ++j) {
					file >> board[i][j];
				}
			}
			gameArchives[ID][archiveID] = board;
		}
		file.close();
	}
	else {
		std::cerr << "Error:����" << ID << "�Ĵ浵�ļ�ʧ��" << std::endl;
	}
}


//���ض�Ӧ�˺ŵĴ浵����
int Database::getArchiveIdInDB(const std::string& inputID){
	
	return gameArchives[inputID].size();
}

//��ȡ�浵��������
sudokuBoard& Database::getBoardInDB(const std::string& inputID, int archiveId){
	return gameArchives[inputID][archiveId-1];
}


//����浵
void Database::addArchiveInDB(const std::string& ID, int archiveId, const sudokuBoard& board) {
	gameArchives[ID][archiveId] = board;
	saveArchivesToFile(ID);
}

//���Ĵ浵
void Database::changeArchiveInDB(const std::string& ID, int archiveId, const sudokuBoard& board) {
	gameArchives[ID][archiveId] = board;
	saveArchivesToFile(ID);
}



//��ȡ����
bool Database::getRankInDB(std::map<std::string, int>&rank)
{
	loadRankFromFile();
	if (rank.size()) {
		rank = this->rank;
		return true;
	}
	else {
		return false;
	}
}

//�˺żӷ�
void Database::addGrade(const std::string&ID) {
	loadRankFromFile();
	++rank[ID];
	std::cout << rank[ID];
}