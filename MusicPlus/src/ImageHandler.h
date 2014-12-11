#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <Qt/QImage.h>

class Filter
{
    //共享同一权重矩阵即可
public:
    Filter(int radius)
    {
        this->radius = radius;
        weights = new double[radius*2+1];
    }

    void destroy()
    {
        if (weights != 0)
            delete [] weights;
        weights = 0;
    }

    int radius;
    double* weights;
};

class ImageHandler
{
public:
    ImageHandler();

    static QImage GaussianBlur(const QImage& image, int radius, double darkness=1.0);//用高斯矩阵处理图像矩阵

    static Filter GaussianFilter(int radius, double darkness=1.0);//获取高斯滤镜,矩阵

    static void useFilter(const Filter& filter, unsigned char* newData, const unsigned char* data, int width, int height, int bytePerLine);//用滤镜处理数据

    static int bytePerLine(int width);//图像每行字节对齐,必须是4字节的倍数
};

#endif // IMAGEHANDLER_H
