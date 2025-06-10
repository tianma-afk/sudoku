#ifndef GAME_2_H
#define GAME_2_H
#include<vector>
using std::vector;

struct Board{
    int data[9][9][2];
};
class dot{
public:
    int x;
    int y;
    bool operator==(const dot&other)const;
};

class Game_2
{
public:
    Game_2();
    Board board[3];
    void newgame();
    void record(const int x,const int y,const int value);
    bool isValid(const int x,const int y,const int value)const;
    bool isExist(const int x,const int y)const;
    bool check();
private:
    int fun_make_done=0;
    bool make(const int row,const int col);
    void makeGong();
    vector<dot> get_bian_jie(const int gong);
    bool swap_bianjie(const dot d,const int gong);
    template<typename T>
    bool is_vector_have(const vector<T>vec_t,const T t)const;
    bool isConnect(const int gong);

    vector<dot>vec_find_around;
    void find_around(const dot d,const int gong);
    void game_debug();
    vector<dot> get_other_bian_jie(const int gong);
    void make_hole();
};

#endif // GAME_2_H
