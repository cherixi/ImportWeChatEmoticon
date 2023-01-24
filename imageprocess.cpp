#include "imageprocess.h"

#include <QPoint>
#include <math.h>
#include <QColor>
#include <QPixmap>
#include <QPainter>
#include <QImage>

#include "declare.h"

ImageProcess::ImageProcess()
{


}

void ImageProcess::getPicturesLayout(QImage image_screen, const QPoint &pos_leftTop, const QPoint &pos_rightBottom)
{
    int pos_first_out_top;   //第一排表情的上外边界
    int pos_last_out_button; //最后一排表情的下外边界
    int pos_first_out_button;    //第一排表情的下外边界
    int pos_second_in_top;    //第二排表情的上内边界

    int pos_first_out_right; //第一列表情的右外边界
    int pos_last_out_left;   //最后一列表情的左外边界
    int pos_antisecond_in_right;  //倒数第二列表情的右内边界

    //一、从左上方点向上按行遍历确定第一排表情的上外边界
    for (int row = pos_leftTop.y(), find = 0; find != 1; row--)
    {
        //在该行从左到右按像素遍历
        for (int col = 1; ; col++)
        {
            QPoint p(col, row);

            //判断该像素颜色与背景颜色相似度
            if (!pixel_compare(image_screen.pixel(p)))
            {
                break;
            }

            //如果遍历到右边界则标记找到边界，退出循环
            if (col ==  screenSize.width() - 1)
            {
                pos_first_out_top = row;
                find = 1;
                break;
            }
        }
    }

    //二、从右下方点向下按行遍历确定最后一排表情的下外边界
    for (int row = pos_rightBottom.y(), find = 0; find != 1; row++)
    {
        //在该行从左到右按像素遍历
        for (int col = 1; ; col++)
        {
            QPoint p(col, row);

            //判断该像素颜色与背景颜色相似度
            if (!pixel_compare(image_screen.pixel(p)))
            {
                break;
            }

            //如果遍历到右边界则标记找到边界，退出循环
            if (col ==  screenSize.width() - 1)
            {
                pos_last_out_button = row;
                find = 1;
                break;
            }
        }
    }

    //三、从左上方点向下按行遍历确定第一排表情的下外边界
    for (int row = pos_leftTop.y(), find = 0; find != 1; row++)
    {
        //在该行从左到右按像素遍历
        for (int col = 1; ; col++)
        {
            QPoint p(col, row);

            //判断该像素颜色与背景颜色相似度
            if (!pixel_compare(image_screen.pixel(p)))
            {
                break;
            }

            //如果遍历到右边界则标记找到边界，退出循环
            if (col ==  screenSize.width() - 1)
            {
                pos_first_out_button = row;
                find = 1;
                break;
            }
        }
    }


    //四、从pos_first_out_button点向下按行遍历确定第二排表情的上内边界
    for (int row = pos_first_out_button + 2, find = 0; find != 1; row++)
    {

        //在该行从左到右按像素遍历
        for (int col = 1; col < screenSize.width() - 1; col++)
        {
            QPoint p(col, row);

            //判断该像素颜色与背景颜色相似度
            if (!pixel_compare(image_screen.pixel(p)))
            {
                pos_second_in_top = row;
                find = 1;
                break;
            }
        }
    }

    //五、从左上方点向右按列遍历确定第一列表情的右外边界
    for (int col = pos_leftTop.x(), find = 0; find != 1; col++)
    {
        //在该列从上到下按像素遍历
        for (int row = pos_leftTop.y(); ; row++)
        {
            QPoint p(col, row);

            //判断该像素颜色与背景颜色相似度
            if (!pixel_compare(image_screen.pixel(p)))
            {
                break;
            }

            //如果遍历到下边界则标记找到边界，退出循环
            if (row ==  pos_rightBottom.y() - 1)
            {
                pos_first_out_right = col;
                find = 1;
                break;
            }
        }
    }

    //六、从右下方点向左按列遍历确定最后一列表情的左外边界
    for (int col = pos_rightBottom.x(), find = 0; find != 1; col--)
    {
        //在该列从上到下按像素遍历
        for (int row = pos_leftTop.y(); ; row++)
        {
            QPoint p(col, row);

            //判断该像素颜色与背景颜色相似度
            if (!pixel_compare(image_screen.pixel(p)))
            {
                break;
            }

            //如果遍历到下边界则标记找到边界，退出循环
            if (row ==  pos_rightBottom.y() - 1)
            {
                pos_last_out_left = col;
                find = 1;
                break;
            }
        }
    }

    //七、从pos_last_out_left点向左按列遍历确定倒数第二列表情的右内边界
    for (int col = pos_last_out_left - 2, find = 0; find != 1; col--)
    {
        //在该列从上到下按像素遍历
        for (int row = pos_leftTop.y(); row < pos_rightBottom.y() - 1; row++)
        {
            QPoint p(col, row);

            //判断该像素颜色与背景颜色相似度
            if (!pixel_compare(image_screen.pixel(p)))
            {
                pos_antisecond_in_right = col;
                find = 1;
                break;
            }
        }
    }

    //获取垂直方向图片数量
    height_picture = pos_first_out_button - pos_first_out_top - 1;
    height_edge = pos_second_in_top - pos_first_out_button;
    num_Vertical =  (pos_last_out_button - pos_first_out_button) / (height_picture + height_edge) + 1;

    //获取水平方向图片数量
    width_picture = pos_first_out_right - 1;
    width_edge = pos_last_out_left - pos_antisecond_in_right;
    num_horizontal = (screenSize.width() + width_edge) / (width_picture + width_edge);

    //获取第一张照片中心
    pos_1stCentre.setX((width_picture + 1) / 2);
    pos_1stCentre.setY((pos_first_out_top + (height_picture + 1) / 2));

}

bool ImageProcess::pixel_compare(const QColor & rgbSelected)
{
    int diff = 25;
    if (abs(rgbSelected.red() - rgbBackground.red()) <= diff && abs(rgbSelected.green() - rgbBackground.green()) <= diff
            && abs(rgbSelected.blue() - rgbBackground.blue()) <= diff)
    {
        return true;
    }
    else
    {
        return false;
    }
}

QPixmap ImageProcess::showPicturesPosition(const QString &path_detailpage)
{
    QImage image;
    image.load(path_detailpage);

    QPainter painter(&image);
    painter.setBrush(QBrush(Qt::red));  //brush画刷填充，setPen可以画圆圈

    for (int row = 0; row < num_Vertical; row++)
    {
        for (int col = 0; col < num_horizontal; col++)
        {
            painter.drawEllipse(pos_1stCentre.x() + col * (height_edge + height_picture) - 50,
                                pos_1stCentre.y() + row * (width_edge + width_picture) - 50,
                                100, 100);
            qDebug() << pos_1stCentre.x() + row * (width_edge + width_picture) - 50 << pos_1stCentre.y() + col * (height_edge + height_picture) - 50;
        }
    }
    image.save("./screenshoot/123.png");
    QPixmap pix = QPixmap::fromImage(image);
    return pix;
}


bool ImageProcess::isButton(QImage &image_pre, QImage &image_now)
{
    int diff = 20;
    QPoint pos;
    pos.setX(pos_1stCentre.x() + (width_picture + width_edge));
    pos.setY(pos_1stCentre.y() + (width_picture + width_edge));
    if (abs(image_pre.pixelColor(pos_1stCentre).red() - image_now.pixelColor(pos_1stCentre).red()) <= diff
            && abs(image_pre.pixelColor(pos_1stCentre).green() - image_now.pixelColor(pos_1stCentre).green()) <= diff
            && abs(image_pre.pixelColor(pos_1stCentre).blue() - image_now.pixelColor(pos_1stCentre).blue()) <= diff

            && abs(image_pre.pixelColor(pos).red() - image_now.pixelColor(pos).red()) <= diff
            && abs(image_pre.pixelColor(pos).green() - image_now.pixelColor(pos).green()) <= diff
            && abs(image_pre.pixelColor(pos).blue() - image_now.pixelColor(pos).blue()) <= diff)
    {
        return true;
    }
    else
    {
        return false;
    }
}


