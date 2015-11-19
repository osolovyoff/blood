#pragma once
#ifndef blood_vision_barrel_hxx_
#define blood_vision_barrel_hxx_

#if defined(USE_OPENCV)
#include <opencv/cv.hpp>

struct Pixel
{
    Pixel()
    : r(0), g(0), b(0), tx(0), ty(0), rn(0), rg(0){}

    unsigned char r;
    unsigned char g;
    unsigned char b;
    int           tx;
    int           ty;
    double        rn;
    double        rg;
};

class Barrel
{
public:
    Barrel(int w, int h);
    ~Barrel();

    void  processingDistortion();
    Pixel getPixel(int x, int y);

    void  recalculateMatrix(const cv::Mat& src, cv::Mat& dst);

private:
    int     m_w;
    int     m_h;
    Pixel** m_pixels;
};

#endif // end USE_OPENCV

#endif // blood_vision_barrel_hxx_