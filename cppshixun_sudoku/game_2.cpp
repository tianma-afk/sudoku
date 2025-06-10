#include "game_2.h"
#include<windows.h>
#include<time.h>
#include<QDebug>
Game_2::Game_2() {
    srand(static_cast<unsigned int>(time(nullptr)));
}

void Game_2::newgame()
{
    qDebug()<<"game:newGame";
    bool done=false;
    while(!done){
        makeGong();
        done=make(0,0);
        fun_make_done=0;
    }

    make_hole();
}

void Game_2::record(const int x, const int y, const int value)
{
    board[1].data[x][y][0]=value;
}

bool Game_2::isValid(const int x, const int y, const int value) const
{
    //该行是否重复
    for(int i=0;i<9;i++){
        if(board[2].data[x][i][0]==value){
            return false;
        }
    }
    //该列是否重复
    for(int i=0;i<9;i++){
        if(board[2].data[i][y][0]==value){
            return false;
        }
    }
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(board[2].data[i][j][1]==board[2].data[x][y][1]){
                if(board[2].data[i][j][0]==value){
                    return false;
                }
            }
        }
    }
    return true;
}

bool Game_2::isExist(const int x, const int y) const
{
    if(board[1].data[x][y][0]==0)
        return false;
    else
        return true;
}

bool Game_2::check()
{
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(!isValid(i,j,board[2].data[i][j][0])){
                return false;
            }
        }
    }
    return true;
}
void Swap(int&a,int&b){
    int tmp=a;
    a=b;
    b=tmp;
}
bool Game_2::make(const int row,const int col)
{
    fun_make_done++;
    if(fun_make_done>30000){
        return false;
    }
    qDebug()<<fun_make_done;
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
        Swap(arr[i],arr[j]);
    }

    //判断继续填充是否可行的巧妙之处在于，如果下次填充能够获得完整数独就认为本次是可行的，如果下次填充不能获得完整数独则本次也不行。
    for (int num : arr) {
        if (isValid(row, col, num)) {
            this->board[2].data[row][col][0] = num;
            if (make(row,col+1)){//判断下次填充是否可得完整数独板
                return true;
            }
            this->board[2].data[row][col][0] = 0;//如果不可，就退回0，再换下一个数字。
        }
    }
    return false;
}

void Game_2::makeGong()
{
    //先形成标准宫
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            board[2].data[i][j][0]=0;
            if(i>=0&&i<=2){
                if(j>=0&&j<=2){
                    board[2].data[i][j][1]=0;
                }else if(j>=3&&j<=5){
                    board[2].data[i][j][1]=1;
                }else{
                    board[2].data[i][j][1]=2;
                }
            }else if(i>=3&&i<=5){
                if(j>=0&&j<=2){
                    board[2].data[i][j][1]=3;
                }else if(j>=3&&j<=5){
                    board[2].data[i][j][1]=4;
                }else{
                    board[2].data[i][j][1]=5;
                }
            }else{
                if(j>=0&&j<=2){
                    board[2].data[i][j][1]=6;
                }else if(j>=3&&j<=5){
                    board[2].data[i][j][1]=7;
                }else{
                    board[2].data[i][j][1]=8;
                }
            }
        }
    }
    //qDebug()<<"game:std";
    //在边界处交换
     for(int n=0;n<9;n++){
        int swap_num=2;
        while(swap_num--){
            vector<dot>bianjie=get_bian_jie(n);
            //for(size_t i=0;i<bianjie.size();i++)
            //qDebug()<<bianjie.at(i).x<<" "<<bianjie.at(i).y;
            //qDebug()<<"game:get_bian_jie";
            int rand_num=rand()%bianjie.size();
            vector<int>have_rand;
            have_rand.push_back(rand_num);
            while(!swap_bianjie(bianjie.at(rand_num),n)){
                if(have_rand.size()==bianjie.size()){
                    break;
                }
                rand_num=rand()%bianjie.size();
                while(is_vector_have(have_rand,rand_num)){
                    rand_num=rand()%bianjie.size();
                }have_rand.push_back(rand_num);
            }
        }
    }
}

vector<dot> Game_2::get_bian_jie(const int gong)
{
    vector<dot>all;
    vector<dot>bian_jie;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(board[2].data[i][j][1]==gong){
                dot tmp;tmp.x=i;tmp.y=j;
                all.push_back(tmp);
            }
        }
    }
    for(int i=0;i<9;i++){
        dot d=all.at(i);
        //上
        if(d.x-1!=-1&&board[2].data[d.x-1][d.y][1]!=gong){
            bian_jie.push_back(d);
        }
        //下
        else if(d.x+1!=9&&board[2].data[d.x+1][d.y][1]!=gong){
            bian_jie.push_back(d);
        }
        //左
        else if(d.y-1!=-1&&board[2].data[d.x][d.y-1][1]!=gong){
            bian_jie.push_back(d);
        }
        //右
        else if(d.y+1!=9&&board[2].data[d.x][d.y+1][1]!=gong){
            bian_jie.push_back(d);
        }
    }
    return bian_jie;
}

vector<dot> Game_2::get_other_bian_jie(const int gong)
{
    vector<dot>all;
    vector<dot>bian_jie;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(board[2].data[i][j][1]==gong){
                dot tmp;tmp.x=i;tmp.y=j;
                all.push_back(tmp);
            }
        }
    }
    for(int i=0;i<9;i++){
        dot d=all.at(i);
        //上
        if(d.x-1!=-1&&board[2].data[d.x-1][d.y][1]!=gong){
            dot tmp;tmp.x=d.x-1;tmp.y=d.y;
            if(!is_vector_have(bian_jie,tmp)){
                bian_jie.push_back(tmp);
            }
        }
        //下
        if(d.x+1!=9&&board[2].data[d.x+1][d.y][1]!=gong){
            dot tmp;tmp.x=d.x+1;tmp.y=d.y;
            if(!is_vector_have(bian_jie,tmp)){
                bian_jie.push_back(tmp);
            }
        }
        //左
        if(d.y-1!=-1&&board[2].data[d.x][d.y-1][1]!=gong){
            dot tmp;tmp.x=d.x;tmp.y=d.y-1;
            if(!is_vector_have(bian_jie,tmp)){
                bian_jie.push_back(tmp);
            }
        }
        //右
        if(d.y+1!=9&&board[2].data[d.x][d.y+1][1]!=gong){
            dot tmp;tmp.x=d.x;tmp.y=d.y+1;
            if(!is_vector_have(bian_jie,tmp)){
                bian_jie.push_back(tmp);
            }
        }
    }
    //for(size_t i=0;i<bian_jie.size();i++){
    //    qDebug()<<bian_jie.at(i).x<<" "<<bian_jie.at(i).y;
    //}
    return bian_jie;
}

void Game_2::make_hole()
{
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            this->board[0].data[i][j][0]=this->board[2].data[i][j][0];
            this->board[1].data[i][j][0]=this->board[2].data[i][j][0];
            this->board[0].data[i][j][1]=this->board[2].data[i][j][1];
            this->board[1].data[i][j][1]=this->board[2].data[i][j][1];
        }
    }
    int empty=30;
    while (empty>0) {
        int row = rand() % 9;
        int col = rand() % 9;
        if (this->board[0].data[row][col][0] != 0) {
            this->board[0].data[row][col][0] = 0;
            this->board[1].data[row][col][0]=0;
            --empty;
        }
    }
}

bool Game_2::swap_bianjie(const dot d,const int gong)
{
    //qDebug()<<"game:swap_bianjie";
    //上
    //game_debug();
    //qDebug()<<d.x<<" "<<d.y<<"  "<<board[2].data[d.x][d.y][1]<<" "<<gong;
    vector<dot>other_bian_jie=get_other_bian_jie(gong);
    for(size_t i=0;i<other_bian_jie.size();i++){
        dot other=other_bian_jie.at(i);
        int o_value=board[2].data[other.x][other.y][1];
        board[2].data[d.x][d.y][1]=o_value;
        board[2].data[other.x][other.y][1]=gong;
        if(isConnect(o_value)&&isConnect(gong)){
            return true;
        }else{
            board[2].data[d.x][d.y][1]=gong;
            board[2].data[other.x][other.y][1]=o_value;
        }
    }
    return false;
}

bool Game_2::isConnect(const int gong)
{
    bool find=false;
    dot begin;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(board[2].data[i][j][1]==gong){
                begin.x=i;begin.y=j;
                find=true;
                break;
            }
        }
        if(find==true){
            break;
        }
    }
    find_around(begin,gong);
    //for(size_t k=0;k<vec_find_around.size();k++)
       // qDebug()<<vec_find_around.at(k).x<<vec_find_around.at(k).y;
    if(vec_find_around.size()==9){
        vec_find_around.clear();
        return true;
    }else{
        vec_find_around.clear();
        return false;
    }
}

void Game_2::find_around(const dot d,const int gong)
{
    //上
    if(d.x-1!=-1&&board[2].data[d.x-1][d.y][1]==gong){
        dot tmp;tmp.x=d.x-1;tmp.y=d.y;
        if(!is_vector_have(vec_find_around,tmp)){
            vec_find_around.push_back(tmp);
            find_around(tmp,gong);
        }
    }
    //下
    if(d.x+1!=9&&board[2].data[d.x+1][d.y][1]==gong){
        dot tmp;tmp.x=d.x+1;tmp.y=d.y;
        if(!is_vector_have(vec_find_around,tmp)){
            vec_find_around.push_back(tmp);
            find_around(tmp,gong);
        }
    }
    //左
    if(d.y-1!=-1&&board[2].data[d.x][d.y-1][1]==gong){
        dot tmp;tmp.x=d.x;tmp.y=d.y-1;
        if(!is_vector_have(vec_find_around,tmp)){
            vec_find_around.push_back(tmp);
            find_around(tmp,gong);
        }
    }
    //右
    if(d.y+1!=9&&board[2].data[d.x][d.y+1][1]==gong){
        dot tmp;tmp.x=d.x;tmp.y=d.y+1;
        if(!is_vector_have(vec_find_around,tmp)){
            vec_find_around.push_back(tmp);
            find_around(tmp,gong);
        }
    }
}

void Game_2::game_debug()
{
    for(int i=0;i<9;i++){
        QString str;
        for(int j=0;j<9;j++){
            str+=QString::number(board[2].data[i][j][1])+" ";
        }
        //qDebug()<<str;
    }
}




template<typename T>
bool Game_2::is_vector_have(const vector<T>vec_t, const T t) const
{
    for(size_t tmp=0;tmp<vec_t.size();tmp++){
        if(t==vec_t.at(tmp)){
            return true;
        }
    }
    return false;
}

bool dot::operator==(const dot &other) const
{
    if(this->x==other.x&&this->y==other.y){
        return true;
    }else{
        return false;
    }
}
