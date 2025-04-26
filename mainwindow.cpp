#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    obj_page_welcome=new page_welcome;
    obj_page_person=new page_person;
    QStackedWidget*stack=new QStackedWidget(this);
    stack->addWidget(obj_page_welcome);
    stack->addWidget(obj_page_person);
    setCentralWidget(stack);
}

MainWindow::~MainWindow()
{

    delete ui;
    delete obj_page_welcome;
    delete obj_page_person;
}


