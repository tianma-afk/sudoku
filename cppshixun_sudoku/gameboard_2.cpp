#include "gameboard_2.h"
#include "ui_gameboard_2.h"
#include<QMessageBox>
GameBoard_2::GameBoard_2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameBoard_2)
{
    ui->setupUi(this);
    initial_board();
}

GameBoard_2::~GameBoard_2()
{
    clear_board();
    delete ui;
}

void GameBoard_2::set_is_local(const bool islocal)
{
    if(islocal){
        is_local=true;
    }else{
        is_local=false;
    }
}

void GameBoard_2::makeCheckOneBtn(const int x, const int y)
{
    cob.state=true;
    cob.x=x;
    cob.y=y;
    cob.originStyleSheet=btn_shuzi_vec.at(cob.x).at(cob.y)->styleSheet();
    btn_shuzi_vec.at(cob.x).at(cob.y)->setStyleSheet(cob.originStyleSheet+"border-image: url(:/img/checked.png);");
}

void GameBoard_2::defaultCheckOneBtn()
{
    if(cob.x==0&&cob.y==0){
        cob.originStyleSheet+=_1_1ss;
    }
    btn_shuzi_vec.at(cob.x).at(cob.y)->setStyleSheet(cob.originStyleSheet);
    cob.state=false;
}

void GameBoard_2::initial_board()
{
    btn_shuzi_vec.resize(9);
    int row_num=0;
    for(vector<QPushButton*>&row:btn_shuzi_vec){
        row.resize(9);
        int col_num=0;
        for(QPushButton*&btn_shuzi:row){
            btn_shuzi=new QPushButton(this);
            btn_shuzi->setGeometry(this->ui->frame_sudoku->x()+col_num*61,this->ui->frame_sudoku->y()+row_num*61,60,60);
            btn_shuzi->setVisible(true);
            btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"font: 500 12pt \"猫啃什锦黑\";");
            if(col_num==0&&row_num==0){
                cob.originStyleSheet=btn_shuzi->styleSheet();
            }
            connect(btn_shuzi,&QPushButton::clicked,this,[=]{
                defaultCheckOneBtn();
                makeCheckOneBtn(row_num,col_num);
                ui->label_loc->setText(QString::number(row_num+1)+"行"+QString::number(col_num+1)+"列被选中");
            });
            col_num++;
        }
        row_num++;
    }

    btn_value_vec.resize(9);
    int value=1;
    for(QPushButton*&btn_value:btn_value_vec){
        btn_value=new QPushButton(this);
        btn_value->setGeometry(1+(value-1)*50,551,42,42);
        btn_value->setVisible(true);
        btn_value->setText(QString::number(value));
        connect(btn_value,&QPushButton::clicked,this,[=]{
            if(ui->label_loc->text().size()==7){
                int row=ui->label_loc->text().at(0).digitValue();
                int col=ui->label_loc->text().at(2).digitValue();
                if(game.isExist(row-1,col-1)){
                    ui->label_loc->setText("固定值");
                }else{
                    if(game.isValid(row-1,col-1,value)){
                        btn_shuzi_vec.at(row-1).at(col-1)->setText(QString::number(value));
                        game.record(row-1,col-1,value);

                        ui->label_loc->setText("未选中");
                    }else{
                        ui->label_loc->setText("重复了");
                    }
                }
            }
        });
        value++;
    }
}

void GameBoard_2::clear_board()
{
    for(vector<QPushButton*>&row:btn_shuzi_vec){
        row.resize(9);
        for(QPushButton*&btn_shuzi:row){
            delete btn_shuzi;
        }
    }
    for(QPushButton*&btn_value:btn_value_vec){
        delete btn_value;
    }
}

void GameBoard_2::on_btn_exit_clicked()
{
    if(is_local==true){
        emit signal_switch_board_to_welcome();
    }else{
        emit GameBoard_2::signal_switch_board_to_person();
    }
}

void GameBoard_2::on_btn_newGame_clicked()
{
    ui->btn_newGame->setEnabled(false);
    game.newgame();
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            QPushButton* btn_shuzi=btn_shuzi_vec.at(i).at(j);
            int value=game.board[0].data[i][j][0];
            btn_shuzi->setText(" ");
            if(value){
                btn_shuzi->setText(QString::number(value));
            }
            switch(game.board[2].data[i][j][1]){
            case 0:btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"background-color:rgba(156, 242, 148,180);");break;
            case 1:btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"background-color:rgba(248, 190, 190,180);");break;
            case 2:btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"background-color:rgba(240, 245, 149,180);");break;
            case 3:btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"background-color:rgba(162, 233, 235,180);");break;
            case 4:btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"background-color:rgba(175, 198, 252,180);");break;
            case 5:btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"background-color:rgba(228, 175, 252,180);");break;
            case 6:btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"background-color:rgba(240, 245, 149,180);");break;
            case 7:btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"background-color:rgba(251, 168, 121,180);");break;
            case 8:btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"background-color:rgba(156, 242, 148,180);");break;
            }
            if(i==0&&j==0){
                _1_1ss=btn_shuzi->styleSheet();
            }
        }
    }

}

void GameBoard_2::on_btn_check_clicked()
{
    bool isOK=game.check();
    if(isOK){
        QMessageBox::information(this,"提示","你真棒！");
    }else{
        QMessageBox::information(this,"提示","别灰心。下次一定能行！");
    }
    ui->btn_newGame->setEnabled(true);
}
