#ifndef PAGE_PERSON_H
#define PAGE_PERSON_H

#include <QWidget>
#include"page.h"
namespace Ui {
class page_person;
}

class page_person : public page
{
    Q_OBJECT

public:
    explicit page_person(page *parent = nullptr);
    ~page_person();

private:
    Ui::page_person *ui;
};

#endif // PAGE_PERSON_H
