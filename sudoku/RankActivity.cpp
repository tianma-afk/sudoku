#include "RankActivity.h"


RankActivity::RankActivity() {
	

}

void RankActivity::showRankActivity(const std::string& ID)
{
	system("cls");
	
	std::cout << "********************���а�******************" << std::endl;
	std::cout << "********************1����*******************" << std::endl;
	std::cout << "******************2չʾ����*****************" << std::endl;
	std::cout << "*****************3�˳����а�****************" << std::endl;
	std::cout << "������:>";
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
				throw std::string("Error:�������");
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
