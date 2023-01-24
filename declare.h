#ifndef DECLARE_H
#define DECLARE_H

#include <QString>
#include <QSize>
#include <QDir>
#include <QColor>
#include <QPoint>


extern QString adbPath;  //adb路径

//有关屏幕尺寸的全局变量
extern QSize screenSize;   //屏幕大小
extern double scale;   //
extern int width_scaled;    //缩放后图片宽度
extern const int  height_scaled; //缩放后图片高度为550
extern QSize size_scaled;   //缩放后图片大小

//有关图像处理的全局变量
extern QColor rgbBackground;    //背景颜色



extern int height_picture;  //表情高
extern int height_edge; //边框高
extern int width_picture;  //表情宽
extern int width_edge; //边框宽

extern double num_horizontal;  //水平方向图片数量
extern double num_Vertical;    //垂直方向图片数量

extern QPoint pos_1stCentre;    //第一张照片的中心

void initGlobalV();

#endif // DECLARE_H
