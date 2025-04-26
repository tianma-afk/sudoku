#ifndef PAGE_H
#define PAGE_H

#include <QWidget>

class page:public QWidget
{
    Q_OBJECT
public:
    explicit page(QWidget*parent=nullptr);
public:
signals:
    void signal_switch_page();
};

#endif // PAGE_H
