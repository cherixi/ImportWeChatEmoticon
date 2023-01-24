#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QPoint>
#include <QImage>


class ImageProcess
{
public:
    ImageProcess();

    static void getPicturesLayout(QImage image_screen, const QPoint &pos_leftTop, const QPoint &pos_rightBottom);

    /**
     * @brief pixel_compare 比较像素与背景像素
     * @param rgbSelected 选定的像素颜色
     * @return bool 是否相似
     */
    static bool pixel_compare(const QColor & rgbSelected);

    static QPixmap showPicturesPosition(const QString &path_detailpage = "./screenshoot/detailPage.png");

    /**
     * @brief isButton 判断屏幕是否到达底部
     * @param image_pre
     * @param image_now
     * @return
     */
    static bool isButton(QImage &image_pre, QImage &image_now);
};

#endif // IMAGEPROCESS_H
