#include<string>
#include<iostream>
#include<map>
#include<fstream>
#include <vector>
using sudokuBoard = std::vector<std::vector<int>>;
#define SUDOKU_SIZE 9

#pragma once

class Database {
private:
	std::map<std::string,std::string>accounts;
	std::map<std::string, std::map<int, sudokuBoard>>gameArchives;
	std::map<std::string, int>rank;

	void saveAccountsToFile();
	void loadAccountsFromFile();
	void saveArchivesToFile(const std::string& ID);
	void loadArchivesFromFile(const std::string& ID);

	void loadRankFromFile();
	void saveRankToFile();


public:
	Database();
	explicit Database(const std::string& inputID);
	bool isIDPasswordMatched(const std::string& inputID, const std::string& inputPassword);
	bool searchInDatabase(const std::string&inputID);
	bool addInDatabase(const std::string& inputID, const std::string& inputPassword);

	void addArchiveInDB(const std::string& ID, int archiveId, const sudokuBoard& board);
	void changeArchiveInDB(const std::string& ID, int archiveId, const sudokuBoard& board);

	int getArchiveIdInDB(const std::string& inputID);
	sudokuBoard& getBoardInDB(const std::string& inputID,const int archiveId);

	bool getRankInDB(std::map<std::string ,int>&rank);
	void addGrade(const std::string& ID);
};

