#ifndef GAMEBOARD_2_H
#define GAMEBOARD_2_H

#include <QWidget>
#include<QPushButton>
#include<vector>
#include"game_2.h"
using std::vector;
namespace Ui {
class GameBoard_2;
}

struct checkOneBtn_2{
    bool state;
    int x;
    int y;
    QString originStyleSheet;
};
class GameBoard_2 : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoard_2(QWidget *parent = nullptr);
    ~GameBoard_2();
    void set_is_local(const bool islocal);
signals:
    void signal_switch_board_to_person();
    void signal_switch_board_to_welcome();
private:
    bool is_local;
    Game_2 game;
    checkOneBtn_2 cob={false,0,0,""};
    void makeCheckOneBtn(const int x,const int y);
    void defaultCheckOneBtn();
    Ui::GameBoard_2 *ui;
    vector<vector<QPushButton*>>btn_shuzi_vec;
    vector<QPushButton*>btn_value_vec;
    QString _1_1ss;
    void initial_board();
    void clear_board();
private slots:
    void on_btn_exit_clicked();
    void on_btn_newGame_clicked();
    void on_btn_check_clicked();
};

#endif // GAMEBOARD_2_H
