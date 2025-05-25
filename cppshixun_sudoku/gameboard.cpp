#include "gameboard.h"
#include "ui_gameboard.h"
#include<QDir>
#include<QTimer>
GameBoard::GameBoard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameBoard)
{
    ui->setupUi(this);
    connect(ui->comboBox_archive,&QComboBox::currentIndexChanged,this,&GameBoard::slot_combox_archive_change);
    initial_board();
    QString localpth=QDir::currentPath()+"/help.pdf";
    helpUrl=QUrl::fromLocalFile(localpth).toString();
}

GameBoard::~GameBoard()
{
    clear_board();
    delete ui;
}

void GameBoard::setUsername(QString username)
{
    this->username=username;
}

void GameBoard::setQTcpSocket(QTcpSocket *server_socket)
{
    this->server_socket=server_socket;
    if(server_socket!=nullptr){
        connect(server_socket,&QTcpSocket::connected,this,&GameBoard::slot_connected);
        connect(server_socket,&QTcpSocket::readyRead,this,&GameBoard::slot_getData);
        slot_connected();
    }
}

void GameBoard::setChat_with_server(chat_with_server *chat)
{
    this->chat=chat;
}

void GameBoard::makeCheckOneBtn(const int x, const int y)
{
    cob.state=true;
    cob.x=x;
    cob.y=y;
    cob.originStyleSheet=btn_shuzi_vec.at(cob.x).at(cob.y)->styleSheet();
    btn_shuzi_vec.at(cob.x).at(cob.y)->setStyleSheet(cob.originStyleSheet+"border-image: url(:/img/checked.png);");
}

void GameBoard::defaultCheckOneBtn()
{
    btn_shuzi_vec.at(cob.x).at(cob.y)->setStyleSheet(cob.originStyleSheet);
    cob.state=false;
}

void GameBoard::initial_board()
{
    if(game.isLaststepsEmpty()){
        ui->btn_laststep->setEnabled(false);
        ui->btn_laststep->setStyleSheet("image: url(:/img/lastStep_disabled.png);");
    }
    if(game.isNextstepsEmpty()){
        ui->btn_nextstep->setEnabled(false);
        ui->btn_nextstep->setStyleSheet("image: url(:/img/nextStep_disabled.png);");
    }
    btn_shuzi_vec.resize(9);
    int row_num=0;
    for(vector<QPushButton*>&row:btn_shuzi_vec){
        row.resize(9);
        int col_num=0;
        for(QPushButton*&btn_shuzi:row){
            btn_shuzi=new QPushButton(this->ui->frame_sudoku);
            btn_shuzi->setGeometry(this->ui->frame_sudoku->x()+col_num*61,this->ui->frame_sudoku->y()+row_num*61,60,60);
            btn_shuzi->setVisible(true);
            btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"font: 500 10pt \"猫啃什锦黑\";");
            if(col_num==row_num||8-col_num==row_num){
                btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"background-color:rgba(255,255,0,180);");
            }
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
                        game.record(row-1,col-1,value,true);
                        if(!game.isLaststepsEmpty()){
                            ui->btn_laststep->setEnabled(true);
                            ui->btn_laststep->setStyleSheet("image: url(:/img/lastStep.png);");
                        }
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

void GameBoard::clear_board()
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

void GameBoard::init_combox_archive()
{
    archive arc;
    QString beginBoard;
    QString nowBoard;
    QString answer;
    int num=chat->jsonData["archiveNum"].toInt();
    this->archives.resize(num);
    for(int n=0;n<num;n++){
        arc.archiveid=chat->jsonData["archiveid"+QString::number(n)].toString();
        beginBoard=chat->jsonData["beginBoard"+QString::number(n)].toString();
        nowBoard=chat->jsonData["nowBoard"+QString::number(n)].toString();
        answer=chat->jsonData["answer"+QString::number(n)].toString();
        for(int i=0;i<9;i++){for(int j=0;j<9;j++){arc.list.data[0][i][j]=beginBoard.at(i*9+j).digitValue();}}
        for(int i=0;i<9;i++){for(int j=0;j<9;j++){arc.list.data[1][i][j]=nowBoard.at(i*9+j).digitValue();}}
        for(int i=0;i<9;i++){for(int j=0;j<9;j++){arc.list.data[2][i][j]=answer.at(i*9+j).digitValue();}}
        this->archives.at(n)=arc;
        ui->comboBox_archive->addItem(arc.archiveid);
    }

    if(archives.size()!=0){
        ui->comboBox_archive->setCurrentText(archives.at(0).archiveid);
        game.setGameBoard(archives.at(0).list);
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                int value=game.getValue(i,j);
                if(value!=0){
                    btn_shuzi_vec.at(i).at(j)->setText(QString::number(value));
                }
            }
        }
        this->isExistGame=true;
        this->isNowGameNew=false;
    }else{
        this->isExistGame=false;
        this->isNowGameNew=false;
    }
}

void GameBoard::on_btn_newgame_clicked()
{
    this->comeFrom_func_newGame_or_save=true;
    if(this->isExistGame){
        QMessageBox ask_is_save_Box(this);
        ask_is_save_Box.setText("是否保存当前文件？");
        ask_is_save_Box.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        ask_is_save_Box.setDefaultButton(QMessageBox::Ok);
        if(ask_is_save_Box.exec()==QMessageBox::Ok){
            oldarc_beforesave.archiveid=ui->comboBox_archive->currentText();
            oldarc_beforesave.list=game.getList();
            this->saveFromnewGame=true;
            on_btn_save_clicked();
        }else{
            if(this->isNowGameNew){
                int index=ui->comboBox_archive->currentIndex();
                ui->comboBox_archive->removeItem(index);
                qDebug()<<"删除未保存的新游戏";
            }
        }
    }
    //给新文档自动取个名字
    if(this->archives.size()==0){
        ui->comboBox_archive->addItem("新存档");
        ui->comboBox_archive->setCurrentText("新存档");
    }else{
        bool haveName=false;
        for(size_t i=0;i<this->archives.size();i++){
            if(archives.at(i).archiveid=="新存档"){
                break;
            }
            if(i==archives.size()-1){
                ui->comboBox_archive->addItem("新存档");
                ui->comboBox_archive->setCurrentText("新存档");
                haveName=true;
            }
        }
        int name_num=1;
        while(!haveName){
            for(size_t i=0;i<this->archives.size();i++){
                if(archives.at(i).archiveid=="新存档_"+QString::number(name_num)){
                    break;
                }
                if(i==archives.size()-1){
                    ui->comboBox_archive->addItem("新存档_"+QString::number(name_num));
                    ui->comboBox_archive->setCurrentText("新存档_"+QString::number(name_num));
                    haveName=true;
                }
            }
        }
    }
    if(game.isLaststepsEmpty()){
        ui->btn_laststep->setEnabled(false);
        ui->btn_laststep->setStyleSheet("image: url(:/img/lastStep_disabled.png);");
    }
    if(game.isNextstepsEmpty()){
        ui->btn_nextstep->setEnabled(false);
        ui->btn_nextstep->setStyleSheet("image: url(:/img/nextStep_disabled.png);");
    }

    QString curText=ui->comboBox->currentText();
    int difficultLevel=1;
    if(curText=="“拿捏”"){
    }else if(curText=="“有点意思”"){
        difficultLevel=2;
    }else if(curText=="“我错了”"){
        difficultLevel=3;
    }else if(curText=="/(ㄒoㄒ)/~~"){
        difficultLevel=4;
    }
    game.setDifficultLevel(difficultLevel);
    game.newgame();
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            int value=game.getValue(i,j);
            if(value!=0){
                btn_shuzi_vec.at(i).at(j)->setText(QString::number(value));
            }else{
                btn_shuzi_vec.at(i).at(j)->setText("");
            }
        }
    }
    this->isExistGame=true;
    this->isNowGameNew=true;
    qDebug()<<"Func:newgame make new";
}

void GameBoard::on_btn_renew_clicked()
{
    if(game.isLaststepsEmpty()){
        ui->btn_laststep->setEnabled(false);
        ui->btn_laststep->setStyleSheet("image: url(:/img/lastStep_disabled.png);");
    }
    if(game.isNextstepsEmpty()){
        ui->btn_nextstep->setEnabled(false);
        ui->btn_nextstep->setStyleSheet("image: url(:/img/nextStep_disabled.png);");
    }
    game.renew();
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            int value=game.getValue(i,j);
            if(value!=0){
                btn_shuzi_vec.at(i).at(j)->setText(QString::number(value));
            }else{
                btn_shuzi_vec.at(i).at(j)->setText("");
            }
        }
    }
}

void GameBoard::on_btn_check_clicked()
{
    if(game.isLaststepsEmpty()){
        ui->btn_laststep->setEnabled(false);
        ui->btn_laststep->setStyleSheet("image: url(:/img/lastStep_disabled.png);");
    }
    if(game.isNextstepsEmpty()){
        ui->btn_nextstep->setEnabled(false);
        ui->btn_nextstep->setStyleSheet("image: url(:/img/nextStep_disabled.png);");
    }
    if(this->isconnection){
        QJsonObject jsonData;
        if(game.check()){
            //成功结算
            jsonData["contentType"]="success";
        }else{
            //失败结算
            jsonData["contentType"]="defeat";
        }
        jsonData["username"]=username;
        jsonData["archiveid"]=ui->comboBox_archive->currentText();

        QString sendMsg=page::jsonToString(jsonData);
        server_socket->write(sendMsg.toUtf8());
    }else{
        QMessageBox::warning(this,"提示","网不好呢");
    }
}

void GameBoard::on_btn_tip_clicked()
{
    if(this->isExistGame){
        step tip_step=game.tip();
        if(tip_step.go){
            emit btn_shuzi_vec.at(tip_step.x).at(tip_step.y)->clicked();
            QString originSheet=btn_value_vec.at(tip_step.value-1)->styleSheet();
            QString tipOrignStyleSheet=ui->btn_tip->styleSheet();
            QTimer *timer =new QTimer(btn_value_vec.at(tip_step.value-1));
            btn_value_vec.at(tip_step.value-1)->setStyleSheet("background-color: rgb(253, 255, 133);");
            ui->btn_tip->setStyleSheet("image: url(:/img/tip_get.png);");
            timer->start(1000);
            connect(timer,&QTimer::timeout,[=](){
                btn_value_vec.at(tip_step.value-1)->setStyleSheet(originSheet);
                ui->btn_tip->setStyleSheet(tipOrignStyleSheet);
                timer->stop();
            });
        }else{
            QMessageBox::information(this,"注意哦！","全部填完，可以判断正误了。");
        }
    }else{
        QMessageBox::warning(this,"注意哦！","游戏还没开始呢！");
    }
}

void GameBoard::on_btn_giveanswer_clicked()
{
    if(game.isLaststepsEmpty()){
        ui->btn_laststep->setEnabled(false);
        ui->btn_laststep->setStyleSheet("image: url(:/img/lastStep_disabled.png);");
    }
    if(game.isNextstepsEmpty()){
        ui->btn_nextstep->setEnabled(false);
        ui->btn_nextstep->setStyleSheet("image: url(:/img/nextStep_disabled.png);");
    }
    game.giveanswer();
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            int value=game.getValue(i,j);
            if(value!=0){
                btn_shuzi_vec.at(i).at(j)->setText(QString::number(value));
            }
        }
    }
}

void GameBoard::on_btn_laststep_clicked()
{
    step s=game.laststep();
    if(s.go){
        if(game.isValid(s.x,s.y,s.value)){
            btn_shuzi_vec.at(s.x).at(s.y)->setText(QString::number(s.value));
        }
    }else{
        btn_shuzi_vec.at(s.x).at(s.y)->setText("");
    }
    if(game.isLaststepsEmpty()){
        ui->btn_laststep->setEnabled(false);
        ui->btn_laststep->setStyleSheet("image: url(:/img/lastStep_disabled.png);");
    }
    if(!game.isNextstepsEmpty()){
        ui->btn_nextstep->setEnabled(true);
        ui->btn_nextstep->setStyleSheet("image: url(:/img/nextStep.png);");
    }
}

void GameBoard::on_btn_nextstep_clicked()
{
    step s=game.nextstep();
    if(s.go){
        if(game.isValid(s.x,s.y,s.value)){
            btn_shuzi_vec.at(s.x).at(s.y)->setText(QString::number(s.value));
            game.record(s.x,s.y,s.value,s.go);
        }
    }else{
        btn_shuzi_vec.at(s.x).at(s.y)->setText("");
    }
    if(game.isNextstepsEmpty()){
        ui->btn_nextstep->setEnabled(false);
        ui->btn_nextstep->setStyleSheet("image: url(:/img/nexttStep_disabled.png);");
    }
    if(!game.isLaststepsEmpty()){
        ui->btn_laststep->setEnabled(true);
        ui->btn_laststep->setStyleSheet("image: url(:/img/lastStep.png);");
    }
}

void GameBoard::on_btn_save_clicked()
{
    if(this->isExistGame==false){
        QMessageBox::information(this,"注意哦！","还没开始游戏呢！");
    }else{
        QString name;
        if(this->isNowGameNew){
            NameDialog dialog(this);
            dialog.setInputName(ui->comboBox_archive->currentText());
            vector<QString>nameList;
            qDebug()<<"new game:1+"<<this->archives.size();
            for(const archive&arc:archives){
                nameList.push_back(arc.archiveid);
            }
            dialog.setNameList(nameList);
            if(dialog.exec()==QDialog::Accepted){
                int index=ui->comboBox_archive->findText(ui->comboBox_archive->currentText());
                name=dialog.getInputName();
                if(name!=ui->comboBox_archive->currentText()){
                    qDebug()<<"名字变了";
                    this->comeFrom_func_newGame_or_save=true;
                    ui->comboBox_archive->setItemText(index,name);
                }else{
                    qDebug()<<"名字没变";
                    this->comeFrom_func_newGame_or_save=false;
                }
            }
        }else{
            name=ui->comboBox_archive->currentText();
        }
        SDKBoardList list;
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                list.data[0][i][j]=game.save().data[0][i][j];
            }
        }
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                list.data[1][i][j]=game.save().data[1][i][j];
            }
        }
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                list.data[2][i][j]=game.save().data[2][i][j];
            }
        }
        if(this->isconnection){
            QJsonObject jsonData;
            jsonData["contentType"]="saveArchive";
            jsonData["username"]=username;
            jsonData["archiveid"]=name;
            QString beginBoard;
            for(int i=0;i<9;i++){for(int j=0;j<9;j++){beginBoard+=QString::number(list.data[0][i][j]);}}
            QString nowBoard;
            for(int i=0;i<9;i++){for(int j=0;j<9;j++){nowBoard+=QString::number(list.data[1][i][j]);}}
            QString answer;
            for(int i=0;i<9;i++){for(int j=0;j<9;j++){answer+=QString::number(list.data[2][i][j]);}}
            jsonData["beginBoard"]=beginBoard;
            jsonData["nowBoard"]=nowBoard;
            jsonData["answer"]=answer;
            QString sendMsg=page::jsonToString(jsonData);
            server_socket->write(sendMsg.toUtf8());
        }else{
            QMessageBox::warning(this,"提示","网不好呢");
        }
    }
}

void GameBoard::on_btn_exit_clicked()
{
    //on_btn_save_clicked();
    QApplication::quit();
}

void GameBoard::on_btn_help_clicked()
{
    if(!QDesktopServices::openUrl(QUrl(helpUrl))){
        QMessageBox::warning(this,"打开帮助文档失败","无法打开浏览器或文档链接无效");
    }
}

void GameBoard::slot_connected()
{
    this->isconnection=true;
    if(this->isconnection){
        QJsonObject jsonData;
        jsonData["contentType"]="getOldArchive";
        jsonData["username"]=this->username;
        QString sendMsg=page::jsonToString(jsonData);
        server_socket->write(sendMsg.toUtf8());
    }else{
        QMessageBox::warning(this,"提示","网不好呢!");
    }
}

void GameBoard::slot_getData()
{
    QByteArray getMsg=this->server_socket->readAll();
    chat->read(getMsg.toStdString());
    QString answer=chat->getAnswer();
    if(answer=="保存成功"){
        QMessageBox::information(this,"保存成功","保存成功");
        if(this->isNowGameNew==true){
            if(this->saveFromnewGame){
                archives.push_back(oldarc_beforesave);
                this->saveFromnewGame=false;
            }else{
                archive arc;
                arc.archiveid=ui->comboBox_archive->currentText();
                arc.list=game.getList();
                archives.push_back(arc);
                this->isNowGameNew=false;
                qDebug()<<"Func:data_after_save make old";
            }
        }else{
            for(size_t i=0;i<this->archives.size();i++){
                if(this->archives.at(i).archiveid==ui->comboBox_archive->currentText()){
                    this->archives.at(i).list=game.getList();
                    break;
                }
            }
        }
    }else if(answer=="保存失败"){
        QMessageBox::information(this,"保存失败","保存失败");
    }else if(answer=="得到存档"){
        init_combox_archive();
    }else if(answer=="结算成功"){
        //设置无效索引
        int index=ui->comboBox_archive->currentIndex();
        ui->comboBox_archive->removeItem(index);
        ui->comboBox_archive->setCurrentIndex(-1);
        if(chat->jsonData["answerType"]=="success"){
            ui->label_loc->setText("成功");
        }else if(chat->jsonData["answerType"]=="defeat"){
            ui->label_loc->setText("失败");
        }
    }else if(answer=="结算失败"){
        QMessageBox::warning(this,"抱歉哦！","结算失败了呢！请稍后重试");
    }

}

void GameBoard::slot_combox_archive_change()
{
    qDebug()<<"被调用";
    qDebug()<<this->comeFrom_func_newGame_or_save;
    if(comeFrom_func_newGame_or_save){
        qDebug()<<"来自新游戏或者保存的换字";
        comeFrom_func_newGame_or_save=false;
    }else{
        on_btn_save_clicked();
        for(size_t i=0;i<archives.size();i++){
            if(archives.at(i).archiveid==ui->comboBox_archive->currentText()){
                game.setGameBoard(archives.at(i).list);
                for(int i=0;i<9;i++){
                    for(int j=0;j<9;j++){
                        int value=game.getValue(i,j);
                        if(value!=0){
                            btn_shuzi_vec.at(i).at(j)->setText(QString::number(value));
                        }else{
                            btn_shuzi_vec.at(i).at(j)->setText("");
                        }
                    }
                }
                this->isExistGame=true;
                this->isNowGameNew=false;
                break;
            }
        }
    }
}
