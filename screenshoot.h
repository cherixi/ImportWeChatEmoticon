#ifndef SCREENSHOOT_H
#define SCREENSHOOT_H

#include <QSize>


class Screenshoot
{
public:
    Screenshoot();


    //检查adb是否连接
    /**
     * @brief adbOK 检查adb是否连接
     * @return bool adb是否连接
     */
    static int adbOK();

    /**
     * @brief shootScreen 屏幕截图并复制到PC
     * @param pngName 截图文件名
     * @param movepath PC路径
     * @param savePath 手机路径
     * @return bool 成功
     */
    static bool shootScreen(const QString &pngName, const QString &movepath = "./screenshoot",const QString &savePath = "/sdcard/");

    /**
     * @brief getSize 获取屏幕基本参数
     */
    static void getSize();

};

#endif // SCREENSHOOT_H
