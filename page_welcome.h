#ifndef PAGE_WELCOME_H
#define PAGE_WELCOME_H
#include"frame_move.h"
#include <QWidget>
#include"page.h"
namespace Ui {
class page_welcome;
}

class page_welcome : public page
{
    Q_OBJECT

public:
    explicit page_welcome(page*parent = nullptr);
    ~page_welcome();

private:
    Ui::page_welcome *ui;
private slots:
    void on_frame_move_left_hovered();
    void on_frame_move_right_hovered();
};

#endif // PAGE_WELCOME_H
