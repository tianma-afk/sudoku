#include"game.h"

Game::Game(const std::string& ID) {
	archiveActivity(ID);
}

//�Ƿ����
void Game::archiveActivity(const std::string&ID){
	while (true)
	{
		archiveID = 1;
		system("cls");
		std::cout << "********************1����********************" << std::endl;
		std::cout << "********************2����Ϸ***************" << std::endl;
		std::cout << "********************3���а�***************" << std::endl;
		std::cout << "********************0�˳���¼***************" << std::endl;
		int choice;
		while (true) {
			std::cout << "��ѡ��:>";
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
					throw std::string("Error:�������");
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


//�ж������Ƿ����
bool Game::isValid(const int& row, const int& col, const int& num)
{
	//�ж�ͬһ���������ظ�
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		if (board[row][i]==num) {
			return false;
		}
	}
	
	//�ж�ͬһ���������ظ�
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		if (board[i][col]==num) {
			return false;
		}
	}

	//�жϾŹ����������ظ�
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
//�ݹ��㷨����������
bool Game::initalBoard(const int& row, const int& col) {
	//�����������ʵ�����һ�񣩣��򴴽����
	if (row == SUDOKU_SIZE) {
		return true;
	}
	//������������һ�С�
	if (col == SUDOKU_SIZE) {
		return initalBoard(row+1,0);
	}
	
	//�������û�����ͼ������
	//Ϊ�˱�֤����ԣ�����������1��9
	int arr[9] = { 1,2,3,4,5,6,7,8,9 };
	for (int i = 0; i < 9; ++i) {
		int j = rand() % 9;
		swap(arr[i],arr[j]);
	}

	//�жϼ�������Ƿ���е�����֮�����ڣ�����´�����ܹ����������������Ϊ�����ǿ��еģ�����´���䲻�ܻ�����������򱾴�Ҳ���С�
	for (int num : arr) {
		if (isValid(row, col, num)) {
			board[row][col] = num;
			if (initalBoard(row,col+1)){//�ж��´�����Ƿ�ɵ�����������
				return true;
			}
			board[row][col] = 0;//������ɣ����˻�0���ٻ���һ�����֡�
		}
	}
	return false;
}


//�����Ѷ��ڿ�
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
		//�Է���һ
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


//�����Ѷ�
void Game::setDifficultLevel(){
	system("cls");
	std::cout << "********************�Ѷ�********************" << std::endl;
	std::cout << "*************1����********2����Ȥ��*********" << std::endl;
	std::cout << "*************3���һ���̫�����ˡ�************" << std::endl;
	while (true) {
		std::cout << "��ѡ���Ѷ�:>";
		std::cin >> difficultLevel;
		try {
			if (difficultLevel < 1 && difficultLevel>3) {
				throw std::string("Error:������Χ");
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

//��ӡ������
void Game::displayBoard(){
	system("cls");
	std::cout << "--------------------������--------------------" << std::endl;
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
	std::cout << "--------------------������--------------------" << std::endl;
}

//��ѡ��
void Game::doChoice(int&choice, const std::string& ID) {
	int row, col, num;
	std::cout << "����0Ϊ�浵�˳�" << std::endl;
	std::cout<<  "����-1ΪУ�Դ�" << std::endl;
	std::cout << "������������Ϊ��λ���ٽ�����һ�����ִ���������ֵ" << std::endl;
	std::cout << "������:>";
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
				std::cout << "���ȫʤ!" << std::endl;
				Database db;
				db.addGrade(ID);
			}
			else {
				std::cout << "ʧ��!" << std::endl;
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
			std::cout << "��������λ�û������֣��ٴ�����:>" << std::endl;
		}
	}
}

//У�Դ�
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

//��Ϸ�д浵
void Game::saveArchiveInGame(const std::string& ID){
	if (isNewArchive()) {
		Database db = Database(ID);
		db.addArchiveInDB(ID, db.getArchiveIdInDB(ID), board);
		std::cout << "�½�һ���浵���˳�������" << std::endl;
	}
	else {
		Database db = Database(ID);
		db.changeArchiveInDB(ID, archiveID, board);
		std::cout << "����һ���浵���˳�������" << std::endl;
	}
	Sleep(3000);
}

//�ж����½��浵���Ǹ���ԭ�д浵
bool Game::isNewArchive() {
	if (archiveID == -1) {
		return false;
	}
	while (true) {
		std::cout << "����һ���µĴ浵��(y/n):>";
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
				throw std::string("Error:������y/n");
			}
		}
		catch (std::string& s) {
			std::cout << s << std::endl;
		}
	}
}

//����
void Game::getArchiveInGame(const std::string& ID) {
	Database db=Database(ID);
	int num = db.getArchiveIdInDB(ID);
	std::cout<<ID<<"�û������˻���"<<num<<"���浵"<<std::endl;
	
	int choice;
	while (true) {
		std::cout << "�������ĸ��浵?\n��ѡ��:>";
		std::cin >> choice;
		try {
			if (choice<1 || choice>num) {
				throw std::string("Error:������Χ");
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


