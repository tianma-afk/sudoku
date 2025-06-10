#include "gameboard.h"
#include "ui_gameboard.h"
#include<QDir>
#include<QTimer>
GameBoard::GameBoard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameBoard)
{
    ui->setupUi(this);
    ui->comboBox_archive->installEventFilter(this);
    connect(ui->comboBox_archive,&QComboBox::currentIndexChanged,this,&GameBoard::slot_combox_archive_change);

    initial_board();
    QString localpth=QDir::currentPath()+"/help.md";
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
        this->isconnection=true;
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
            btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"font: 500 12pt \"猫啃什锦黑\";");
            if(
                (col_num<3&&row_num<3)||
                (col_num<3&&row_num>5)||
                (col_num>5&&row_num<3)||
                (col_num>5&&row_num>5)||
                (col_num>=3&&col_num<=5&&row_num>=3&&row_num<=5)
                )
            {
                btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"background-color:rgba(156, 242, 148,180);");
            }else{
                btn_shuzi->setStyleSheet(btn_shuzi->styleSheet()+"background-color:rgba(246, 237, 182,180);");
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
        this->old_index_of_combox_archive=ui->comboBox_archive->currentIndex();
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
    if(this->isExistGame==true){
            oldarc_beforesave.archiveid=ui->comboBox_archive->currentText();
            oldarc_beforesave.list=game.getList();
            this->save_from_newGame=true;
            qDebug()<<"来自新游戏的保存，设置为true";
            on_btn_save_clicked();
    }
    //给新文档自动取个名字
    qDebug()<<"被点击";
    if(this->archives.size()==0){
        this->index_change_from_newGame=true;
        qDebug()<<"新游戏改变index，设置为true";
        ui->comboBox_archive->addItem("新存档");
        ui->comboBox_archive->setCurrentText("新存档");
    }else{
        bool haveName=false;
        for(size_t i=0;i<this->archives.size();i++){
            if(archives.at(i).archiveid=="新存档"){
                break;
            }
            if(i==archives.size()-1){
                this->index_change_from_newGame=true;
                qDebug()<<"新游戏改变index，设置为true";
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
    if(this->save_from_newGame==true){
        qDebug()<<"因为创建新游戏，进行保存";
        QString name=oldarc_beforesave.archiveid;
        if(this->isNowGameNew==true){
            qDebug()<<"这是个新游戏，需要起个名字";
            NameDialog dialog(this);
            dialog.setInputName(name);
            vector<QString>nameList;
            for(const archive&arc:archives){nameList.push_back(arc.archiveid);}
            dialog.setNameList(nameList);
            if(dialog.exec()==QDialog::Accepted){
                int index=ui->comboBox_archive->currentIndex();
                name=dialog.getInputName();
                ui->comboBox_archive->setItemText(index,name);
                qDebug()<<"将新存档命名为"<<name;
            }
        }else{
            qDebug()<<"这是个老游戏";
        }
        SDKBoardList list=oldarc_beforesave.list;
        oldarc_beforesave.archiveid=name;
        oldarc_beforesave.list=list;
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
    }else if(this->save_from_arc_change==true){
        qDebug()<<"因为更换存档，进行保存";
        QString name=ui->comboBox_archive->itemText(this->old_index_of_combox_archive);
        if(this->isNowGameNew==true){
            qDebug()<<"这是个新游戏，需要起个名字";
            NameDialog dialog(this);
            dialog.setInputName(name);
            vector<QString>nameList;
            for(const archive&arc:archives){nameList.push_back(arc.archiveid);}
            dialog.setNameList(nameList);
            if(dialog.exec()==QDialog::Accepted){
                int index=this->old_index_of_combox_archive;
                name=dialog.getInputName();
                ui->comboBox_archive->setItemText(index,name);
                qDebug()<<"将新存档命名为"<<name;
            }
        }else{
            qDebug()<<"这是个老游戏";
        }
        SDKBoardList list=game.getList();
        oldarc_beforesave.archiveid=name;
        oldarc_beforesave.list=list;
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
    }else{
        qDebug()<<"因为点击保存按钮，进行保存";
        QString name=ui->comboBox_archive->currentText();
        if(this->isNowGameNew==true){
            qDebug()<<"这是个新游戏，需要起个名字";
            NameDialog dialog(this);
            dialog.setInputName(name);
            vector<QString>nameList;
            for(const archive&arc:archives){nameList.push_back(arc.archiveid);}
            dialog.setNameList(nameList);
            if(dialog.exec()==QDialog::Accepted){
                int index=ui->comboBox_archive->currentIndex();
                name=dialog.getInputName();
                ui->comboBox_archive->setItemText(index,name);
                qDebug()<<"将新存档命名为"<<name;
            }
        }else{
            qDebug()<<"这是个老游戏";
        }
        SDKBoardList list=game.getList();
        oldarc_beforesave.archiveid=name;
        oldarc_beforesave.list=list;
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
    disconnect(server_socket,&QTcpSocket::readyRead,this,&GameBoard::slot_getData);
    this->isconnection=false;
    emit GameBoard::signal_switch_board_to_person();
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
        if(this->save_from_newGame==true){
            qDebug()<<"服务端因为新游戏的保存成功";
            this->save_from_newGame=false;
            qDebug()<<"创建新新游戏成功，oldindex改变";
            this->old_index_of_combox_archive=ui->comboBox_archive->currentIndex();
            for(size_t t=0;t<archives.size();t++){
                if(archives.at(t).archiveid==oldarc_beforesave.archiveid){
                    archives.at(t).list=oldarc_beforesave.list;
                    qDebug()<<"这是老游戏";
                    break;
                }
                if(t==archives.size()-1){
                    archives.push_back(oldarc_beforesave);
                    qDebug()<<"这是新游戏";
                }
            }
        }else if(this->save_from_arc_change==true){
            qDebug()<<"服务端因为更换存档的保存成功";
            this->save_from_arc_change=false;
            for(size_t t=0;t<archives.size();t++){
                if(archives.at(t).archiveid==oldarc_beforesave.archiveid){
                    archives.at(t).list=oldarc_beforesave.list;
                    qDebug()<<"这是老游戏";
                    break;
                }
                if(t==archives.size()-1){
                    archives.push_back(oldarc_beforesave);
                    qDebug()<<"这是新游戏";
                }
            }
        }else{
            qDebug()<<"服务端因为点击保存按钮的保存成功";
            if(this->isNowGameNew==true){
                this->isNowGameNew=false;
                qDebug()<<"这是新游戏";
                qDebug()<<"点击保存使得新游戏变旧";
                archives.push_back(oldarc_beforesave);
            }else{
                qDebug()<<"这是老游戏";
                for(size_t t=0;t<archives.size();t++){
                    if(archives.at(t).archiveid==oldarc_beforesave.archiveid){
                        archives.at(t).list=oldarc_beforesave.list;
                        break;
                    }
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
    if(this->index_change_from_newGame==true){
        qDebug()<<"因为创建新游戏，改变了index";
        this->index_change_from_newGame=false;
    }else{
        qDebug()<<"因为更换存档，改变了index";
        this->save_from_arc_change=true;
        on_btn_save_clicked();
        for(size_t t=0;t<archives.size();t++){
            if(archives.at(t).archiveid==ui->comboBox_archive->currentText()){
                game.setGameBoard(archives.at(t).list);
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
    this->old_index_of_combox_archive=ui->comboBox_archive->currentIndex();
}

void GameBoard::slot_disconnect()
{
    qDebug()<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    this->isconnection=false;
}


