#include "namedialog.h"
#include "ui_namedialog.h"
#include<QMessageBox>
NameDialog::NameDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NameDialog)
{
    ui->setupUi(this);
    connect(ui->btn_ok,&QPushButton::clicked,this,&NameDialog::on_btn_Ok_clicked);
}

NameDialog::~NameDialog()
{
    delete ui;
}

QString NameDialog::getInputName() const
{
    return ui->lineEdit->text();
}

void NameDialog::setNameList(const vector<QString> &nameList)
{
    this->nameList=nameList;
}

void NameDialog::setInputName(const QString&originName)
{
    ui->lineEdit->setText(originName);
}

void NameDialog::on_btn_Ok_clicked()
{
    if(!ui->lineEdit->text().isEmpty()){
        for(const QString&name:this->nameList){
            if(name==ui->lineEdit->text()){
                QMessageBox::warning(this,"注意哦！","存档名已存在");
                ui->lineEdit->setText("");
                return;
            }
        }
        accept();
    }else{
        QMessageBox::warning(this,"注意哦！","存档名不能为空");
        return;
    }
}

void NameDialog::closeEvent(QCloseEvent *event)
{
    int num=1;
    bool isOK=false;
    int size=this->nameList.size();
    qDebug()<<size;
    if(size==0){
        ui->lineEdit->setText("存档_"+QString::number(1));
    }else{
        while(!isOK){
            for(int i=0;i<size;i++){
                if(this->nameList.at(i)=="存档_"+QString::number(num)){
                    num++;
                    break;
                }
                if(i==size-1){
                    ui->lineEdit->setText("存档_"+QString::number(num));
                    isOK=true;
                }
            }
        }
    }
    event->ignore();
    this->accept();
}


