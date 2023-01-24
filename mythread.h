#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include<QPoint>

class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread(QPoint pos_add,  QPoint pos_chooseAlbum,
             QPoint pos_album,  QPoint pos_determine, bool repeat);

    void init(QPoint pos_add,  QPoint pos_chooseAlbum,
              QPoint pos_album,  QPoint pos_determine, bool repeat);

    QPoint pos_add,pos_chooseAlbum,pos_album,pos_leftTop,pos_determine,pos_rightBottom;
    int num_added;
    bool repeat;
    bool flag;


    int row, col, num_repeat;

    MyThread();

protected:
    virtual void run();

signals:
    void sig_num_added(int num_added);

};

#endif // MYTHREAD_H
