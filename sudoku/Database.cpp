#include"database.h"

//数据库源文件
//数据库初始化
Database::Database() {
	loadAccountsFromFile();
}
 Database::Database(const std::string&inputID){
	loadArchivesFromFile(inputID);
}

//校验ID与密码是否匹配
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

//在数据库中查找ID是否存在
bool Database::searchInDatabase(const std::string& inputID)
{
	for (const auto& account : accounts) {
		if (account.first == inputID) {
				return true;
		}
	}
	return false;
}

//添加账号并在排名中加入该用户
bool Database::addInDatabase(const std::string& inputID, const std::string& inputPassword){
	if (searchInDatabase(inputID)) {
		std::cout << "Attention:用户ID已存在" << std::endl;
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



//保存账号到文件
void Database::saveAccountsToFile() {
	std::ofstream file("accounts.txt");
	if (file.is_open()) {
		for (const auto& account : accounts) {
			file << account.first<< " " << account.second<<std::endl;
		}
		file.close();
	}
	else {
		std::cerr << "Error:将账号保存到文件失败" << std::endl;
	}
}

//加载账号文件
void Database::loadAccountsFromFile() {
	std::ifstream file("accounts.txt");
	if (file.is_open()) {
		std::string ID, password;
		while (file >> ID >> password) {
			accounts[ID] = password;
		}
	}
	else {
		std::cerr << "Error:加载账号文件失败" << std::endl;
	}
}

//保存数独存档到文件
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
		std::cerr << "Error:将" << ID << "的存档保存到文件失败" << std::endl;
	}	
}

//将排名从文件中加载出来
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
		std::cerr << "Error:加载排名文件失败" << std::endl;
	}
}

//保存排名到文件中
void Database::saveRankToFile() {
	std::ofstream file("rank.txt");
	if (file.is_open()) {
		for (const auto& perRank : rank) {
			file << perRank.first << " " << perRank.second << std::endl;
		}
		file.close();
	}
	else {
		std::cerr << "Error:将排名保存到文件失败" << std::endl;
	}
}



//加载文件中的数独存档
void Database::loadArchivesFromFile(const std::string& ID) {
	std::ifstream file(ID + ".txt");
	if (file.is_open()) {
		int archiveID;
		while (file >> archiveID) {
			sudokuBoard board(SUDOKU_SIZE, std::vector<int>(SUDOKU_SIZE)); // 初始化二维vector
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
		std::cerr << "Error:加载" << ID << "的存档文件失败" << std::endl;
	}
}


//返回对应账号的存档数量
int Database::getArchiveIdInDB(const std::string& inputID){
	
	return gameArchives[inputID].size();
}

//获取存档的数独板
sudokuBoard& Database::getBoardInDB(const std::string& inputID, int archiveId){
	return gameArchives[inputID][archiveId-1];
}


//加入存档
void Database::addArchiveInDB(const std::string& ID, int archiveId, const sudokuBoard& board) {
	gameArchives[ID][archiveId] = board;
	saveArchivesToFile(ID);
}

//更改存档
void Database::changeArchiveInDB(const std::string& ID, int archiveId, const sudokuBoard& board) {
	gameArchives[ID][archiveId] = board;
	saveArchivesToFile(ID);
}



//获取排名
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

//账号加分
void Database::addGrade(const std::string&ID) {
	loadRankFromFile();
	++rank[ID];
	std::cout << rank[ID];
}