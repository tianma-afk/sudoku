/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *pte_show;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_start;
    QPushButton *btn_stop;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(690, 447);
        widget = new QWidget(Widget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(-3, -10, 701, 451));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pte_show = new QPlainTextEdit(widget);
        pte_show->setObjectName("pte_show");
        pte_show->setStyleSheet(QString::fromUtf8("font: 14pt \"Consolas\";"));

        verticalLayout->addWidget(pte_show);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btn_start = new QPushButton(widget);
        btn_start->setObjectName("btn_start");

        horizontalLayout->addWidget(btn_start);

        btn_stop = new QPushButton(widget);
        btn_stop->setObjectName("btn_stop");

        horizontalLayout->addWidget(btn_stop);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        btn_start->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213", nullptr));
        btn_stop->setText(QCoreApplication::translate("Widget", "\345\205\263\351\227\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
