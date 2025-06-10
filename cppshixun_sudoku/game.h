#ifndef GAME_H
#define GAME_H
#include<vector>
using std::vector;
#include"sdkboardlist.h"
struct step{
    int x;
    int y;
    int value;
    bool go;
};

class Game
{
public:
    Game();
    void newgame();
    step laststep();
    step nextstep();
    step tip();
    bool check();
    void giveanswer();
    void renew();
    void record(const int x,const int y,const int value,const bool go);
    void setDifficultLevel(const int difficultLevel);
    int getValue(const int x,const int y)const;
    bool isValid(const int x,const int y,const int value)const;
    bool isExist(const int x,const int y)const;
    bool isLaststepsEmpty();
    bool isNextstepsEmpty();
    bool setGameBoard(const SDKBoardList&list);
    SDKBoardList getList();
private:
    void MakeABoard();
    bool make(const int row,const int col);
    void generateEmpty();
    int difficultLevel=1;//默认难度是1
    SDKBoardList list;
    vector<step>laststeps;
    vector<step>nextsteps;
};

#endif // GAME_H
