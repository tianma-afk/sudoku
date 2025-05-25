#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include"page.h"
#include <QWidget>
#include<QPushButton>
#include<vector>
#include"game.h"
#include<QDesktopServices>
#include<QUrl>
#include<QTcpSocket>
#include"chat_with_server.h"
#include<QMessageBox>
#include"namedialog.h"
using std::vector;
namespace Ui {
class GameBoard;
}
struct archive{
    SDKBoardList list;
    QString archiveid;
};
struct checkOneBtn{
    bool state;
    int x;
    int y;
    QString originStyleSheet;
};

class GameBoard : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoard(QWidget *parent = nullptr);
    ~GameBoard();
    void setUsername(QString username);
    void setQTcpSocket(QTcpSocket*server_socket);
    void setChat_with_server(chat_with_server*chat);
private:
    bool comeFrom_func_newGame_or_save;
    bool saveFromnewGame=false;
    archive oldarc_beforesave;

    checkOneBtn cob={false,0,0,""};
    void makeCheckOneBtn(const int x,const int y);
    void defaultCheckOneBtn();
    int old_index_of_combox_archive=0;
    bool isExistGame=false;
    bool isNowGameNew=false;
    QString username;
    void initial_board();
    void clear_board();
    void init_combox_archive();
    vector<vector<QPushButton*>>btn_shuzi_vec;
    vector<QPushButton*>btn_value_vec;
    Ui::GameBoard *ui;
    Game game;
    QString helpUrl;
    bool isconnection;
    chat_with_server*chat;
    QTcpSocket*server_socket;
    vector<archive>archives;
private slots:
    void on_btn_newgame_clicked();
    void on_btn_renew_clicked();
    void on_btn_check_clicked();
    void on_btn_tip_clicked();
    void on_btn_giveanswer_clicked();
    void on_btn_laststep_clicked();
    void on_btn_nextstep_clicked();
    void on_btn_save_clicked();
    void on_btn_exit_clicked();
    void on_btn_help_clicked();
    void slot_connected();
    void slot_getData();
    void slot_combox_archive_change();
};

#endif // GAMEBOARD_H
