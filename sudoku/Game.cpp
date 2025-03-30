#include"game.h"

Game::Game(const std::string& ID) {
	archiveActivity(ID);
}

//是否读档
void Game::archiveActivity(const std::string&ID){
	while (true)
	{
		archiveID = 1;
		system("cls");
		std::cout << "********************1读档********************" << std::endl;
		std::cout << "********************2新游戏***************" << std::endl;
		std::cout << "********************3排行榜***************" << std::endl;
		std::cout << "********************0退出登录***************" << std::endl;
		int choice;
		while (true) {
			std::cout << "请选择:>";
			std::cin >> choice;
			try {
				switch (choice) {
				case 1:
					getArchiveInGame(ID);
				case 2:
					setDifficultLevel();
					board = std::vector<std::vector<int>>(9, std::vector<int>(9, 0));
					srand(static_cast<unsigned int>(time(nullptr)));
					initalBoard(0, 0);
					generateEmpty();
					break;
				case 3:
				{
					RankActivity rankActivity;
					if (rankActivity.getRankInRank()) {
						rankActivity.showRankActivity(ID);
					}

					break;
				}
				case 0:
					return;
				default:
					throw std::string("Error:输入错误");
				}
			}
			catch (const std::string& s) {
				std::cout << s << std::endl;
			}
			int choice;
			do {
				displayBoard();
				doChoice(choice, ID);
			} while (choice);
		}
	}
}


//判断输入是否合理
bool Game::isValid(const int& row, const int& col, const int& num)
{
	//判断同一行中有无重复
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		if (board[row][i]==num) {
			return false;
		}
	}
	
	//判断同一列中有无重复
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		if (board[i][col]==num) {
			return false;
		}
	}

	//判断九宫格中有无重复
	int rowGroup = row / 3;
	int colGroup = col / 3;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (board[row/3*3+i][col/3*3+j] == num){
				return false;
			}
		}
	}
	return true;
}

void swap(int& a,int& b) {
	int tmp = a;
	a = b;
	b = tmp;
}
//递归算法创建数独板
bool Game::initalBoard(const int& row, const int& col) {
	//如果行满格（其实是溢出一格），则创建完成
	if (row == SUDOKU_SIZE) {
		return true;
	}
	//如果列满格，则加一行。
	if (col == SUDOKU_SIZE) {
		return initalBoard(row+1,0);
	}
	
	//如果都还没满，就继续填充
	//为了保证随机性，先造出乱序的1到9
	int arr[9] = { 1,2,3,4,5,6,7,8,9 };
	for (int i = 0; i < 9; ++i) {
		int j = rand() % 9;
		swap(arr[i],arr[j]);
	}

	//判断继续填充是否可行的巧妙之处在于，如果下次填充能够获得完整数独就认为本次是可行的，如果下次填充不能获得完整数独则本次也不行。
	for (int num : arr) {
		if (isValid(row, col, num)) {
			board[row][col] = num;
			if (initalBoard(row,col+1)){//判断下次填充是否可得完整数独板
				return true;
			}
			board[row][col] = 0;//如果不可，就退回0，再换下一个数字。
		}
	}
	return false;
}


//根据难度挖空
void Game::generateEmpty(){
	int empty = 0;
	switch (difficultLevel) {
	case 1:
		empty = 30;
		break;
	case 2:
		empty = 50;
		break;
	case 3:
		empty = 70;
		break;
	default:
		//以防万一
		empty = 30;
		break;
	}
	while (empty>0) {
		int row = rand() % 9;
		int col = rand() % 9;
		if (board[row][col] != 0) {
			board[row][col] = 0;
			--empty;
		}
	}
}


//设置难度
void Game::setDifficultLevel(){
	system("cls");
	std::cout << "********************难度********************" << std::endl;
	std::cout << "*************1拿捏********2“有趣”*********" << std::endl;
	std::cout << "*************3“我还是太年轻了”************" << std::endl;
	while (true) {
		std::cout << "请选择难度:>";
		std::cin >> difficultLevel;
		try {
			if (difficultLevel < 1 && difficultLevel>3) {
				throw std::string("Error:超出范围");
			}
			else {
				break;
			}
		}
		catch (const std::string& s) {
			std::cout << s<<std::endl;
		}
		
	}
}

//打印数独板
void Game::displayBoard(){
	system("cls");
	std::cout << "--------------------数独板--------------------" << std::endl;
	std::cout << " ";
	for (int i = 1; i <=9; ++i) {
		std::cout << i;
	}
	std::cout << std::endl;
	for (int i = 0; i < 9; ++i) {
		std::cout << i+1;
		for (int j = 0; j < 9; ++j) {
			if (board[i][j] == 0) {
				std::cout << " ";
			}
			else {
				std::cout << board[i][j];
			}
		}
		std::cout << std::endl;
	}
	std::cout << "--------------------数独板--------------------" << std::endl;
}

//做选择
void Game::doChoice(int&choice, const std::string& ID) {
	int row, col, num;
	std::cout << "输入0为存档退出" << std::endl;
	std::cout<<  "输入-1为校对答案" << std::endl;
	std::cout << "输入有序数对为点位，再接下来一个数字代表填入数值" << std::endl;
	std::cout << "请输入:>";
	while(true){
		std::cin >> row;
		if (row == 0) {
			choice = 0;
			saveArchiveInGame(ID);
			return;
		}
		else if (row == -1) {
			choice = 0;
			if (isAnswerMatched()) {
				std::cout << "大获全胜!" << std::endl;
				Database db;
				db.addGrade(ID);
			}
			else {
				std::cout << "失败!" << std::endl;
			}
			return;
		}
		std::cin >> col;
		std::cin >> num;
		if (row > 0 && row < 10 && col>0 && col < 10 && num>0 && num < 10) {
			board[row - 1][col - 1] = num;
			return;
		}
		else {
			std::cout << "输入错误的位置或者数字，再次输入:>" << std::endl;
		}
	}
}

//校对答案
bool Game::isAnswerMatched(){
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			if (!isValid(i, j, board[i][j])) {
				return false;
			}
		}
	}
	return true;
}

//游戏中存档
void Game::saveArchiveInGame(const std::string& ID){
	if (isNewArchive()) {
		Database db = Database(ID);
		db.addArchiveInDB(ID, db.getArchiveIdInDB(ID), board);
		std::cout << "新建一个存档，退出。。。" << std::endl;
	}
	else {
		Database db = Database(ID);
		db.changeArchiveInDB(ID, archiveID, board);
		std::cout << "覆盖一个存档，退出。。。" << std::endl;
	}
	Sleep(3000);
}

//判断是新建存档还是覆盖原有存档
bool Game::isNewArchive() {
	if (archiveID == -1) {
		return false;
	}
	while (true) {
		std::cout << "创建一个新的存档吗？(y/n):>";
		char choice;
		std::cin >> choice;
		try
		{
			if (choice == 'y') {
				return true;
			}
			else if (choice == 'n') {
				return false;
			}
			else {
				throw std::string("Error:请输入y/n");
			}
		}
		catch (std::string& s) {
			std::cout << s << std::endl;
		}
	}
}

//读档
void Game::getArchiveInGame(const std::string& ID) {
	Database db=Database(ID);
	int num = db.getArchiveIdInDB(ID);
	std::cout<<ID<<"用户您的账户有"<<num<<"个存档"<<std::endl;
	
	int choice;
	while (true) {
		std::cout << "您想玩哪个存档?\n请选择:>";
		std::cin >> choice;
		try {
			if (choice<1 || choice>num) {
				throw std::string("Error:超出范围");
			}
			else {
				break;
			}
		}
		catch(std::string&s){
			std::cout << s << std::endl;
		}
	}
	board = db.getBoardInDB(ID, choice);
	archiveID = choice - 1;
}


