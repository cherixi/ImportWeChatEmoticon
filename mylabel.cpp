#include "mylabel.h"

#include <QDebug>
#include <QMouseEvent>

MyLabel::MyLabel(QWidget *parent)
    : QLabel{parent}
{
}


void MyLabel::mousePressEvent(QMouseEvent *ev)
{
    QPoint p = ev->pos();
    qDebug() << p;
    emit clicked(p);
}
