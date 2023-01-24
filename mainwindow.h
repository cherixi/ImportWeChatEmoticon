#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>

#include "mythread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPoint pos_add,pos_chooseAlbum,pos_album,pos_leftTop,pos_determine,pos_rightBottom;

    int adbStatus = 0;

    MyThread mythread;

    //!!!是否删除!!!
    double pcScale = 1.5;
protected:
    void keyPressEvent(QKeyEvent *event);

    void init();


private:
    Ui::MainWindow *ui;

public slots:
};
#endif // MAINWINDOW_H
