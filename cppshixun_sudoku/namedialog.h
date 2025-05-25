#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include <QDialog>
#include<vector>
#include<QCloseEvent>
using std::vector;
namespace Ui {
class NameDialog;
}

class NameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NameDialog(QWidget *parent = nullptr);
    ~NameDialog();

    QString getInputName()const;
    void setNameList(const vector<QString>&nameList);
    void setInputName(const QString&originName);
private slots:
    void on_btn_Ok_clicked();
    void closeEvent(QCloseEvent*event)override;
private:
    Ui::NameDialog *ui;
    vector<QString>nameList;
};

#endif // NAMEDIALOG_H
