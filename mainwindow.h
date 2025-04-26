#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"page_welcome.h"
#include"page_person.h"
#include<QStackedWidget>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    page_welcome*obj_page_welcome;
    page_person*obj_page_person;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
