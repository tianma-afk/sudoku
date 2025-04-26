#ifndef FRAME_MOVE_H
#define FRAME_MOVE_H
#include<QFrame>
#include<QEvent>
class frame_move:public QFrame
{
    Q_OBJECT
public:
    frame_move(QWidget*parent=nullptr);
protected:
    void enterEvent(QEnterEvent*event)override;
signals:
    void signal_hover();
};

#endif // FRAME_MOVE_H
