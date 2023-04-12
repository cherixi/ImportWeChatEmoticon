#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <QCursor>
#include "winuser.h"
#include <QThread>
#include <QPushButton>
#include <QPixmap>
#include <QMessageBox>

#include "declare.h"
#include "cmdcomm.h"
#include "screenshoot.h"
#include "imageprocess.h"

#include "mythread.h"

extern void initGlobalV();  //引入全局函数


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    this->setWindowIcon(QIcon(":/res/icon.png"));

    //!!!检查lambda中是否有改变参数值：mutable!!!
    //!!!daemon not running; starting now at tcp:5037!!!

    //初始化label_adb
    adbStatus = Screenshoot::adbOK();
    switch (adbStatus)
    {
    case 0:
        ui->textBrowser_adb->setTextColor(QColor(255,0,0));
        ui->textBrowser_adb->setText("失败：未连接，未授权");
        break;
    case 1:
        ui->textBrowser_adb->setTextColor(QColor(255,0,0));
        ui->textBrowser_adb->setText("失败：已连接，未授权");
        break;
    case 2:
        ui->textBrowser_adb->setTextColor(QColor(0,255,0));
        ui->textBrowser_adb->setText("成功：已连接，已授权");
        break;
    case 3:
        ui->textBrowser_adb->setTextColor(QColor(255,0,0));
        ui->textBrowser_adb->setText("错误：请重试");
    default:
        break;
    }

    //点击退出按钮，退出程序
    connect(ui->btn_exit, &QPushButton::clicked, this, [=](){
        qDebug() << "colse: triggered by btn_exit";
        this->close();
    });

    //点击重新开始按钮，重置程序
    connect(ui->btn_reset, &QPushButton::clicked, this, [=](){
        qDebug() << "start: reset";
        ui->textBrowser_debug->append("info: run reset");

        //重置成员变量
        this->init();

        //重置全局变量
        initGlobalV();

        //回到主界面
        ui->stackedWidget->setCurrentWidget(ui->page_home);
    });

    //点击更新按钮，检查adb连接状态
    connect(ui->btn_adbcheck, &QPushButton::clicked, this, [=](){
        adbStatus = Screenshoot::adbOK();
        switch (adbStatus)
        {
        case 0:
            ui->textBrowser_adb->setTextColor(QColor(255,0,0));
            ui->textBrowser_adb->setText("失败：未连接，未授权");
            break;
        case 1:
            ui->textBrowser_adb->setTextColor(QColor(255,0,0));
            ui->textBrowser_adb->setText("失败：已连接，未授权");
            break;
        case 2:
            ui->textBrowser_adb->setTextColor(QColor(0,255,0));
            ui->textBrowser_adb->setText("成功：已连接，已授权");
            break;
        case 3:
            ui->textBrowser_adb->setTextColor(QColor(255,0,0));
            ui->textBrowser_adb->setText("错误：请重试");
        default:
            break;
        }
    });

    //点击开始按钮，进入初始前界面
    connect(ui->btn_start, &QPushButton::clicked, this, [=](){
        //检查adb连接状态
        if(adbStatus == 2)
        {
            ui->label_show1->setPixmap(QPixmap(":/res/1.jpg"));
            ui->stackedWidget->setCurrentWidget(ui->page_pre);
        }
        //adb错误时给出提示
        switch (adbStatus)
        {
        case 0:
            QMessageBox::critical(this, "失败", "设备未连接，未授权");
            return;
            break;
        case 1:
            QMessageBox::critical(this, "失败", "设备已连接，未授权");
            return;
            break;
        case 3:
            QMessageBox::critical(this, "失败", "设备连接错误：请更新设备连接状态");
            return;
            break;
        default:
            break;
        }
        ui->textBrowser_debug->append("info: adb fail");
    });


    //在初始前界面中点击继续按钮，进入初始界面
    connect(ui->btn_toInit, &QPushButton::clicked, this, [=](){
        Screenshoot::shootScreen("screen.png");
        Screenshoot::getSize();
        ui->label->resize(size_scaled);

        QPixmap pix_screen("./screenshoot/screen.png");
        QPixmap dest = pix_screen.scaled(ui->label->size(),Qt::KeepAspectRatio);
        ui->label->setPixmap(dest);
        ui->label_show2->setPixmap(QPixmap(":/res/2.jpg"));
        ui->label_show8->setPixmap(QPixmap(":/res/8.jpg"));
        ui->stackedWidget->setCurrentWidget(ui->page_init);
    });

    //在初始界面中记录点击位置，进入图片库界面
    connect(ui->label, &MyLabel::clicked, this, [=](QPoint clickPos){
        pos_add = clickPos * scale; //记录点击位置

        //模拟点击
        QString s = QString("adb shell input swipe %1 %2 %1 %2 50").arg(pos_add.x()).arg(pos_add.y());
        CmdComm::runcmd(s);

        Sleep(1000);

        //重新截图并传输
        Screenshoot::shootScreen("screen.png");
        ui->label_2->resize(size_scaled);
        QPixmap pix_screen("./screenshoot/screen.png");
        QPixmap dest = pix_screen.scaled(ui->label_2->size(),Qt::KeepAspectRatio);
        ui->label_2->setPixmap(dest);
        ui->label_show3->setPixmap(QPixmap(":/res/3.jpg"));
        ui->label_show9->setPixmap(QPixmap(":/res/9.jpg"));
        ui->stackedWidget->setCurrentWidget(ui->page_picLib);

    });

    //在图片库界面中记录点击位置，进入相册选择界面
    connect(ui->label_2, &MyLabel::clicked, this, [=](QPoint clickPos){
        pos_chooseAlbum = clickPos * scale; //记录点击位置



        //模拟点击
        QString s = QString("adb shell input swipe %1 %2 %1 %2 50").arg(pos_chooseAlbum.x()).arg(pos_chooseAlbum.y());
        CmdComm::runcmd(s);

        Sleep(1000);

        //重新截图
        Screenshoot::shootScreen("screen.png");
        ui->label_3->resize(size_scaled);
        QPixmap pix_screen("./screenshoot/screen.png");

        //获取背景颜色
        QImage img_screen = pix_screen.toImage();
        rgbBackground.setRgb(img_screen.pixel(screenSize.width() - pos_chooseAlbum.x(), pos_chooseAlbum.y()));

        QPixmap dest = pix_screen.scaled(ui->label_3->size(),Qt::KeepAspectRatio);
        ui->label_3->setPixmap(dest);
        ui->label_show4->setPixmap(QPixmap(":/res/4.jpg"));
        ui->stackedWidget->setCurrentWidget(ui->page_chooseAlbum);
    });


    //在相册选择界面中记录点击位置,进入详情1界面
    connect(ui->label_3, &MyLabel::clicked, this, [=](QPoint clickPos){
        pos_album = clickPos * scale;
        QString s = QString("adb shell input swipe %1 %2 %1 %2 50").arg(pos_album.x()).arg(pos_album.y());
        CmdComm::runcmd(s);
        Sleep(1000);

        Screenshoot::shootScreen("detailPage.png");
        ui->label_4->resize(size_scaled);

        QPixmap pix_screen("./screenshoot/detailPage.png");
        QPixmap dest = pix_screen.scaled(ui->label_4->size(),Qt::KeepAspectRatio);
        ui->label_4->setPixmap(dest);
        ui->label_show5->setPixmap(QPixmap(":/res/5.jpg"));
        ui->stackedWidget->setCurrentWidget(ui->page_detail1);
    });

    //在详情1界面中记录点击位置，进入详情2界面
    connect(ui->label_4, &MyLabel::clicked, this, [=](QPoint clickPos){
        pos_leftTop = clickPos * scale;

        ui->label_5->resize(size_scaled);

        QPixmap pix_screen("./screenshoot/detailPage.png");
        QPixmap dest = pix_screen.scaled(ui->label_5->size(), Qt::KeepAspectRatio);
        ui->label_5->setPixmap(dest);
        ui->label_show6->setPixmap(QPixmap(":/res/6.jpg"));
        ui->stackedWidget->setCurrentWidget(ui->page_detail2);
    });

    //在详情2界面中记录点击位置，进入确定界面
    connect(ui->label_5, &MyLabel::clicked, this, [=](QPoint clickPos){
        pos_rightBottom = clickPos * scale; //记录点击位置

        //模拟点击左上角图片
        QString s = QString("adb shell input swipe %1 %2 %1 %2 50").arg(pos_leftTop.x()).arg(pos_leftTop.y());
        CmdComm::runcmd(s);
        Sleep(1000);

        //截图并载入
        Screenshoot::shootScreen("screen.png");
        ui->label_6->resize(size_scaled);
        QPixmap pix_screen("./screenshoot/screen.png");
        QPixmap dest = pix_screen.scaled(ui->label_5->size(), Qt::KeepAspectRatio);
        ui->label_6->setPixmap(dest);
        ui->label_show7->setPixmap(QPixmap(":/res/7.jpg"));
        ui->label_show10->setPixmap(QPixmap(":/res/10.jpg"));
        ui->stackedWidget->setCurrentWidget(ui->page_right);
    });

    //在确定界面中记录点击位置，进入显示图片位置界面
    connect(ui->label_6, &MyLabel::clicked, this, [=](QPoint clickPos){
        pos_determine = clickPos * scale;

        QString s = QString("adb shell input swipe %1 %2 %1 %2 50")
                .arg(pos_determine.x())
                .arg(pos_determine.y());
        CmdComm::runcmd(s);

        QImage image;
        image.load("./screenshoot/detailPage.png");
        ImageProcess::getPicturesLayout(image, pos_leftTop, pos_rightBottom);
        ui->label_7->resize(size_scaled);

        QPixmap pix_screen = ImageProcess::showPicturesPosition();
        QPixmap dest = pix_screen.scaled(ui->label_7->size(),Qt::KeepAspectRatio);
        ui->label_7->setPixmap(dest);
        ui->stackedWidget->setCurrentWidget(ui->page_showPos);
    });

    //在显示图片位置界面点击只添加选中的表情按钮，进入开始界面
    connect(ui->btn_addPage, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->page_running);

        mythread.init(pos_add, pos_chooseAlbum, pos_album, pos_determine, false);
        mythread.start();
    });

    //在显示图片位置界面点击自动滚动，添加整个相册按钮，进入开始界面
    connect(ui->btn_addAlbum, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->page_running);

        mythread.init(pos_add, pos_chooseAlbum, pos_album, pos_determine, true);
        mythread.start();
    });


    //点击暂停按钮，暂停子线程
    connect(ui->btn_pause, &QPushButton::clicked, this, [=](){
        if (mythread.flag == false)
        {
            mythread.flag = true;
            ui->btn_pause->setText("暂停");
            ui->textBrowser_debug->append("info: continue");
            mythread.start();
        }
        else
        {
            mythread.flag = false;
            ui->btn_pause->setText("继续");
            ui->textBrowser_debug->append("info: pause");
        }
    });

    //子线程报告循环次数
    connect(&mythread, &MyThread::sig_num_added, this, [=](int num_added){
        ui->label_num_added->setText(QString::number(num_added));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Q)
    {
        qDebug() << "关闭程序：由Q键触发";
        this->close();
    }

    if(event->key() == Qt::Key_C)
    {
        qDebug() << rgbBackground.red() <<rgbBackground.green()<<rgbBackground.blue();
    }
}

void MainWindow::init()
{
    //重置成员变量
    pos_add.setX(0); pos_add.setY(0);
    pos_chooseAlbum.setX(0);  pos_chooseAlbum.setY(0);
    pos_album.setX(0); pos_album.setY(0);
    pos_leftTop.setX(0); pos_leftTop.setY(0);
    pos_determine.setX(0); pos_determine.setY(0);
    pos_rightBottom.setX(0); pos_rightBottom.setY(0);
}

