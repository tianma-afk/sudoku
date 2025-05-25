#include "game.h"
#include<windows.h>
#include<time.h>
#include<QDebug>
Game::Game() {
    srand(static_cast<unsigned int>(time(nullptr)));
}

void Game::newgame()
{
    MakeABoard();
}

step Game::laststep()
{
    step s=laststeps.back();
    laststeps.pop_back();
    if(s.go)
        list.data[1][s.x][s.y]=0;
    else
        list.data[1][s.x][s.y]=s.value;
    s.go=!s.go;
    nextsteps.push_back(s);

    return s;
}

step Game::nextstep()
{
    step s=nextsteps.back();
    nextsteps.pop_back();
    s.go=!s.go;
    return s;
}

void Game::record(const int x, const int y, const int value,const bool go)
{
    step s;
    s.x=x;
    s.y=y;
    s.value=value;
    s.go=go;
    nextsteps.clear();
    laststeps.push_back(s);
    if(go)
        list.data[1][x][y]=value;
    else
        list.data[1][x][y]=0;
}

SDKBoardList Game::save()
{
    SDKBoardList list;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            list.data[0][i][j]=this->list.data[0][i][j];
        }
    }
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            list.data[1][i][j]=this->list.data[1][i][j];
        }
    }
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            list.data[2][i][j]=this->list.data[2][i][j];
        }
    }
    return list;
}

step Game::tip()
{
    step tip_step;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(list.data[1][i][j]==0){
                for(int v=1;v<=9;v++){
                    if(isValid(i,j,v)){
                        tip_step.go=true;
                        tip_step.value=v;
                        tip_step.x=i;
                        tip_step.y=j;
                        return tip_step;
                    }
                }
            }
        }
    }
    tip_step.go=false;
    return tip_step;
}

bool Game::check()
{
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            ;
            if(!isValid(i,j,QChar::fromLatin1(list.data[1][i][j]).digitValue())){
                qDebug()<<i<<j<<list.data[1][i][j];
                return false;}
        }
    }
    laststeps.clear();
    nextsteps.clear();
    return true;
}

void Game::giveanswer()
{
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            list.data[1][i][j]=list.data[2][i][j];
        }
    }
    laststeps.clear();
    nextsteps.clear();
}

void Game::renew()
{
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            list.data[1][i][j]=list.data[0][i][j];
        }
    }
    laststeps.clear();
    nextsteps.clear();
}



void Game::setDifficultLevel(const int difficultLevel)
{
    this->difficultLevel=difficultLevel;
}

int Game::getValue(const int x, const int y) const
{
    return list.data[1][x][y];
}

bool Game::isValid(const int x,const int y,const int value)const
{
    //判断同一行中有无重复
    for (int i = 0; i < 9; ++i) {
        if (list.data[1][x][i]==value) {
            return false;
        }
    }

    //判断同一列中有无重复
    for (int i = 0; i <9; ++i) {
        if (list.data[1][i][y]==value) {
            return false;
        }
    }

    //判断九宫格中有无重复
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (list.data[1][x/3*3+i][y/3*3+j] == value){
                return false;
            }
        }
    }
    return true;
}

bool Game::isExist(const int x, const int y) const
{
    if(list.data[0][x][y]!=0){
        return true;
    }else{
        return false;
    }
}

bool Game::isLaststepsEmpty()
{
    return laststeps.empty();
}

bool Game::isNextstepsEmpty()
{
    return nextsteps.empty();
}

bool Game::setGameBoard(const SDKBoardList &list)
{
    for(int i=0;i<9;i++){for(int j=0;j<9;j++){this->list.data[0][i][j]=list.data[0][i][j];}}
    for(int i=0;i<9;i++){for(int j=0;j<9;j++){this->list.data[1][i][j]=list.data[1][i][j];}}
    for(int i=0;i<9;i++){for(int j=0;j<9;j++){this->list.data[2][i][j]=list.data[2][i][j];}}
    return true;
}

SDKBoardList Game::getList()
{
    return list;
}

void swap(int&a,int&b){
    int tmp=a;
    a=b;
    b=tmp;
}

bool Game::make(const int row,const int col){
    //如果行满格（其实是溢出一格），则创建完成
    if (row == 9) {
        return true;
    }
    //如果列满格，则加一行。
    if (col == 9) {
        return make(row+1,0);
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
            this->list.data[1][row][col] = num;
            if (make(row,col+1)){//判断下次填充是否可得完整数独板
                return true;
            }
            this->list.data[1][row][col] = 0;//如果不可，就退回0，再换下一个数字。
        }
    }
    return false;
}

void Game::MakeABoard()
{
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            this->list.data[1][i][j]=0;
        }
    }
    make(0,0);
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            this->list.data[2][i][j]=this->list.data[1][i][j];
        }
    }
    generateEmpty();
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            this->list.data[0][i][j]=this->list.data[1][i][j];
        }
    }
}

void Game::generateEmpty(){
    int empty = 0;
    switch (difficultLevel) {
    case 1:
        empty = 30;
        break;
    case 2:
        empty = 45;
        break;
    case 3:
        empty = 60;
        break;
    case 4:
        empty=75;
        break;
    default:
        //以防万一
        empty = 30;
        break;
    }
    while (empty>0) {
        int row = rand() % 9;
        int col = rand() % 9;
        if (this->list.data[1][row][col] != 0) {
            this->list.data[1][row][col] = 0;
            --empty;
        }
    }
}
