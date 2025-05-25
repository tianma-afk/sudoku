#include "frame_move.h"

frame_move::frame_move(QWidget*parent):QFrame(parent)
{

}
void frame_move::enterEvent(QEnterEvent*event){
    emit signal_hover();
    QWidget::enterEvent(event);
}

