#include "RankActivity.h"


RankActivity::RankActivity() {
	

}

void RankActivity::showRankActivity(const std::string& ID)
{
	system("cls");
	
	std::cout << "********************排行榜******************" << std::endl;
	std::cout << "********************1搜索*******************" << std::endl;
	std::cout << "******************2展示所有*****************" << std::endl;
	std::cout << "*****************3退出排行榜****************" << std::endl;
	std::cout << "请输入:>";
	int choice;
	while (true) {
		std::cin >> choice;
		try {
			switch (choice) {
			case 1:
				searchInRank(ID);
				break;
			case 2:
				showInRank();
				break;
			case 3:
				return;
			default:
				throw std::string("Error:输入错误");
				break;
			}
		}
		catch (std::string& s) {
			std::cout << s << std::endl;
		}
	}
}



bool RankActivity::getRankInRank() {
	Database db;
	if (db.getRankInDB(rank)) {
		return true;
	}
	else {
		return false;
	}
}

void RankActivity::searchInRank(const std::string& ID)
{
	std::cout << ID << " " << rank[ID] << std::endl;
}

void RankActivity::showInRank()const
{
	std::vector<std::string, int>gradeRank(rank.begin(), rank.end());
	
	int num = gradeRank.size();
	for (int i = 0; i < num-1; ++i) {
		if (gradeRank[i] < gradeRank[i + 1])
		{
			int tmp = gradeRank[i + 1];

		}
	}

}
