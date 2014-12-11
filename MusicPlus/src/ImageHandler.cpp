#include "ImageHandler.h"
#include <math.h>
#include <qdebug.h>
#include<time.h>
#define PI 3.1415

//除了圆形对称之外，高斯模糊也可以在二维图像上对两个独立的一维空间分别进行计算，这叫作线性可分。

ImageHandler::ImageHandler()
{
}

QImage ImageHandler::GaussianBlur(const QImage& image, int radius, double darkness)
{
    long start=clock();
    Filter filter = GaussianFilter(radius, darkness);
    QImage ret(image.width(), image.height(), QImage::Format_RGB32);

    useFilter(filter, ret.bits(), image.bits(), image.width(), image.height(), image.bytesPerLine());
    filter.destroy();

    qDebug()<<"run time: "<<clock()-start<<"ms";
    return ret;
}

void ImageHandler::useFilter(const Filter& filter, unsigned char* newData, const unsigned char* data, int width, int height, int bytePerLine)
{
    double* weights = filter.weights;
    int radius = filter.radius;
    unsigned char* buff = new unsigned char[bytePerLine*height];

    //对原始data矩阵进行横向处理
    for (int j=0; j<height; j++)
    {
        int offset = j*bytePerLine;
        for (int i=0; i<width; i++)
        {
            double r=0.0, g=0.0, b=0.0;
            int n = 0;//追踪权重在filter中的位置
            for (int x=-radius; x<=radius; x++)
            {
                int ii = i + x;
                //边缘处理
                if (ii<0 || ii>=width)
                    ii = i;
                b += data[offset+4*ii+0] * weights[n];
                g += data[offset+4*ii+1] * weights[n];
                r += data[offset+4*ii+2] * weights[n];
                n++;
            }
            buff[offset+4*i+0] = b;
            buff[offset+4*i+1] = g;
            buff[offset+4*i+2] = r;
        }
    }

    //对中间buff矩阵进行竖直处理
    for (int i=0; i<width; i++)
    {
        for (int j=0; j<height; j++)
        {
            double r=0.0, g=0.0, b=0.0;
            int n = 0;//追踪权重在filter中的位置
            for (int y=-radius; y<=radius; y++)
            {
                int jj = j + y;
                //边缘处理
                if (jj<0 || jj>=height)
                    jj = j;
                b += buff[jj*bytePerLine+4*i+0] * weights[n];
                g += buff[jj*bytePerLine+4*i+1] * weights[n];
                r += buff[jj*bytePerLine+4*i+2] * weights[n];
                n++;
            }
            newData[j*bytePerLine+4*i+0] = b;
            newData[j*bytePerLine+4*i+1] = g;
            newData[j*bytePerLine+4*i+2] = r;
        }
    }

    delete [] buff;
}

Filter ImageHandler::GaussianFilter(int radius, double darkness)
{
    Filter filter(radius);

    double sigma = radius / 3;//3*sigma之后的权重非常小,忽略
    double sigma22 = sigma * sigma * 2.0;
    double alpha = sigma * sqrt(PI*2.0);
    double sum = 0;

    double* p = filter.weights;
    for (int x=-radius; x<=radius; x++)
    {
        *p = exp( -(double) x*x/sigma22 ) / alpha;
        sum += *p;
        p++;
    }

    qDebug()<<sum;
    p = filter.weights;
    sum = sum / darkness;
    for (int x=-radius; x<=radius; x++)
    {
        *p = *p / sum;
        p++;
    }


//    for (int n=0; n<radius*2+1; n++)
//    {
//        printf("%lf\t", filter.weights[n]);
//    }
//    printf("\n");

    return filter;
}

int ImageHandler::bytePerLine(int width)
{
    //W = ( w * bitcount + 31 )/32 * 4;
    return (width*32 + 31) / 32 * 4;
}
