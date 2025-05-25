#include "page_person.h"
#include "ui_page_person.h"

page_person::page_person(page *parent)
    : page(parent)
    , ui(new Ui::page_person)
{
    ui->setupUi(this);

}

page_person::~page_person()
{
    delete ui;
}
