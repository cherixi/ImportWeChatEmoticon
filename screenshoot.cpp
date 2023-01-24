#include "screenshoot.h"
#include <QProcess>
#include <QDebug>
#include <QSize>

#include <cmdcomm.h>
#include "declare.h"

Screenshoot::Screenshoot()
{
}

int Screenshoot::adbOK()
{
    qDebug() << "start: check adbOK";

    QString outputDevices = CmdComm::runcmd("adb devices");

    bool unauthorized = outputDevices.contains("unauthorized");
    if(unauthorized)
    {
        qDebug() << "unauthorized";
        return 1;
    }
    bool device = outputDevices.contains("device\r\n\r\n");
    if(device)
    {
        qDebug() << "device";
        return 2;
    }
    bool none = outputDevices.contains("List of devices attached\r\n\r\n");
    if(none)
    {
        qDebug() <<"none";
        return 0;
    }
    qDebug() << "error:no match in adbOK";
    return -1;
}

bool Screenshoot::shootScreen(const QString &pngName, const QString &movePath, const QString &savePath)
{
    //格式化命令
    QString command_shoot = QString("adb shell screencap -p %1%2").arg(savePath).arg(pngName);
    QString command_copy = QString("adb pull %1%2 %3").arg(savePath).arg(pngName).arg(movePath);

    //屏幕截图
    CmdComm::runcmd(command_shoot);

    //传输照片到PC
    QString transmissionStatus = CmdComm::runcmd(command_copy);

    //判断是否成功
    if(transmissionStatus.contains("1 file pulled"))
    {
        qDebug() << "sucess: shoot screen";
        return true;
    }
    else
    {
        qDebug() << "error: shoot screen fail";
        return false;
    }

}

void Screenshoot::getSize()
{
    qDebug() << "start: get screen size";
    QString screenSize_cmd = CmdComm::runcmd("adb shell wm size");
    int x_pos = screenSize_cmd.indexOf('x');

    int width_screen = screenSize_cmd.mid(14, x_pos-14).toInt();
    int height_screen = screenSize_cmd.mid(x_pos + 1).toInt();

    screenSize.setWidth(width_screen);
    screenSize.setHeight(height_screen);

    scale = (double)height_screen / height_scaled;
    width_scaled = width_screen / scale;
    size_scaled.setHeight(height_scaled);
    size_scaled.setWidth(width_scaled);

    qDebug() << "success: get screensize: " << screenSize.width() << "x" << screenSize.height();
    qDebug() << "success: get scale: " << scale;
    qDebug() << "success: get width_scaled: " << width_scaled;
}
