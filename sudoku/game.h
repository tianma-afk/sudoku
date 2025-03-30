#include <vector>
#include<string>
#include<iostream>
#include<windows.h>
#include "database.h"
#include"RankActivity.h"

#pragma once

class Game {
private:
	sudokuBoard answer;
	sudokuBoard board;
	int archiveID;
	int difficultLevel;

	bool isValid(const int& row,const int& col,const int& num);
	bool initalBoard(const int& row, const int& col);
	void generateEmpty();
	bool isNewArchive();
	void archiveActivity(const std::string& ID);
	
public:
	Game(const std::string& ID);
	
	void doChoice(int&choice, const std::string& ID);
	void setDifficultLevel();
	void displayBoard();
	bool isAnswerMatched();
	void saveArchiveInGame(const std::string& ID);
	void getArchiveInGame(const std::string& ID);
};
