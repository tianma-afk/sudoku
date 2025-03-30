#pragma once
#include"database.h"

//≈≈––∞Ò“≥√Ê
class RankActivity {
private:
	std::map<std::string,int>rank;
	

	void searchInRank(const std::string&ID);
	void showInRank()const ;
public:
	bool getRankInRank();
	RankActivity();
	void showRankActivity(const std::string& ID);
};