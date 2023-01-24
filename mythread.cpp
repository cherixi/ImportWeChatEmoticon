#include "mythread.h"

#include <synchapi.h>
#include <QString>
#include <QPoint>
#include <QImage>

#include "cmdcomm.h"
#include "declare.h"
#include "screenshoot.h"
#include "imageprocess.h"


MyThread::MyThread()
{

}

MyThread::MyThread(QPoint pos_add,  QPoint pos_chooseAlbum,
                    QPoint pos_album,  QPoint pos_determine, bool repeat)
{
    this->pos_add = pos_add;
    this->pos_chooseAlbum = pos_chooseAlbum;
    this->pos_album = pos_album;
    this->pos_determine = pos_determine;
    this->repeat = repeat;
    num_added = 0;
}

void MyThread::init(QPoint pos_add,  QPoint pos_chooseAlbum,
                    QPoint pos_album,  QPoint pos_determine, bool repeat)
{
    this->pos_add = pos_add;
    this->pos_chooseAlbum = pos_chooseAlbum;
    this->pos_album = pos_album;
    this->pos_determine = pos_determine;
    this->repeat = repeat;
    num_added = 0;
    flag = true;
    row = col = num_repeat = 0;
}

void MyThread::run()
{
    for (; ; )
    {
        for (; row < num_Vertical; ++row)
        {
            if (col == num_horizontal)
            {
                col = 0;
            }

            for (; col < num_horizontal; ++col)
            {
                //滑动屏幕
                for (int i = 0; i < num_repeat; i++)
                {
                    QString s = QString("adb shell input swipe %1 %2 %1 %3 4000")
                            .arg(pos_1stCentre.x())
                            .arg(pos_1stCentre.y() + (height_picture + height_edge))
                            .arg(pos_1stCentre.y());
                    CmdComm::runcmd(s); //模拟滑动
                }

                qDebug() << "开始：" << col << row;

                Sleep(5000);
                //模拟点击加号
                qDebug() << "模拟点击加号";
                QString s = QString("adb shell input swipe %1 %2 %1 %2 50")
                        .arg(pos_add.x()).arg(pos_add.y());
                CmdComm::runcmd(s);

                Sleep(2000);

                //模拟点击选择相册
                qDebug() << "模拟点击选择相册";
                s = QString("adb shell input swipe %1 %2 %1 %2 50")
                        .arg(pos_chooseAlbum.x()).arg(pos_chooseAlbum.y());
                CmdComm::runcmd(s);

                Sleep(2000);

                //模拟点击相册
                qDebug() << "模拟点击相册";
                s = QString("adb shell input swipe %1 %2 %1 %2 50")
                        .arg(pos_album.x()).arg(pos_album.y());
                CmdComm::runcmd(s);

                Sleep(2000);

                //模拟点击表情
                qDebug() << "模拟点击表情";
                s = QString("adb shell input swipe %1 %2 %1 %2 50")
                        .arg(pos_1stCentre.x() + col * (width_edge + width_picture))
                        .arg(pos_1stCentre.y() + row * (height_edge + height_picture));
                CmdComm::runcmd(s);

                Sleep(3000);

                //模拟点击确定
                qDebug() << "模拟点击确定";
                s = QString("adb shell input swipe %1 %2 %1 %2 50")
                        .arg(pos_determine.x()).arg(pos_determine.y());
                CmdComm::runcmd(s);

                num_added++;
                emit sig_num_added(num_added);

                if (!flag)
                {
                    col++;
                    break;
                }
                Sleep(5000);
            }
            if (!flag)
            {
                if (col == num_horizontal)
                {
                    row++;
                }
                break;
            }

        }
        if (!flag)
        {
            break;
        }
        if (repeat)
        {
            //进入相册
            qDebug() << "开始：" << col << row;

            Sleep(5000);
            //模拟点击加号
            qDebug() << "模拟点击加号";
            QString s = QString("adb shell input swipe %1 %2 %1 %2 50")
                    .arg(pos_add.x()).arg(pos_add.y());
            CmdComm::runcmd(s);

            Sleep(2000);

            //模拟点击选择相册
            qDebug() << "模拟点击选择相册";
            s = QString("adb shell input swipe %1 %2 %1 %2 50")
                    .arg(pos_chooseAlbum.x()).arg(pos_chooseAlbum.y());
            CmdComm::runcmd(s);

            Sleep(2000);

            //模拟点击相册
            qDebug() << "模拟点击相册";
            s = QString("adb shell input swipe %1 %2 %1 %2 50")
                    .arg(pos_album.x()).arg(pos_album.y());
            CmdComm::runcmd(s);

            //截图，模拟滑动，再次截图，判断是否到底
            Screenshoot::shootScreen("image_pre.png");  //读取现有截图
            s = QString("adb shell input swipe %1 %2 %1 %3 4000")
                    .arg(pos_1stCentre.x())
                    .arg(pos_1stCentre.y() + (num_Vertical - 1) * (height_picture + height_edge))
                    .arg(pos_1stCentre.y());
            CmdComm::runcmd(s); //模拟滑动
            Screenshoot::shootScreen("image_now.png");  //再次截图

            //判断是否到底
            QImage image_pre, image_now;
            image_pre.load("./screenshoot/image_pre.png");
            image_now.load("./screenshoot/image_now.png");
            bool isButton = ImageProcess::isButton(image_pre, image_now);

            if (isButton)
            {
                break;
            }
            else
            {
                num_repeat++;
                row = 1;
                continue; 
            }
        }
        else
        {
            break;
        }
    }


}
