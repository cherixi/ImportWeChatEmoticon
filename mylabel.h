#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = nullptr);

    //鼠标点击事件
    virtual void mousePressEvent(QMouseEvent *ev);

signals:
    void clicked(QPoint clickPos);
};

#endif // MYLABEL_H
