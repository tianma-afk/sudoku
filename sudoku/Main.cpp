#include"account.h"
#include"game.h"
using namespace std;

int main() {
	int accountChoice = 0;


	while (true) {
		system("cls");
		cout << "你好！欢迎游玩sudoku！" << endl;
		cout << "好吧。它就是数独，但是简单的游戏也许有别样的魅力呢！" << endl;
		cout << "祝你游玩愉快！" << endl;
		cout << "**********1注册账号**********" << endl;
		cout << "**********2登录账号**********" << endl;
		cout << "**********3退出游戏**********" << endl;
		cout << "选择:>";
		cin >> accountChoice;
		try
		{
			switch (accountChoice)
			{
			case 1: {
				RegisterActivity account = RegisterActivity();
				Game(account.getID());
				break;
			}
			case 2: {
				LoginActivity account = LoginActivity();
				Game(account.getID());
				break;
			}
			case 3: {
				std::cout << "欢迎下次再来玩哦ヾ(˙▽˙)Bye~Bye~" << std::endl;
				return 0;
			}
			default:
				throw std::string("请输入1/2/3");
			}
		}
		catch(std::string&s){
			std::cout << s << std::endl;
		}
	}
}