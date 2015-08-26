#if defined(USE_OPENCV)
#include "vision/barrel.hxx"

#include <blood>
#include <opencv2/opencv.hpp>

#include <math.h>
#include <vector>
#include <array>
// TODO matrix deformation
Barrel::Barrel(int w, int h)
: m_pixels(nullptr)
, m_w(w)
, m_h(h)
{
    m_pixels = new Pixel*[h];
    for (int i = 0; i < m_h; ++i)
        m_pixels[i] = new Pixel[w];
}

Barrel::~Barrel()
{
    for (int i = 0; i < m_h; ++i)
        delete[] m_pixels[i];
    delete[] m_pixels;
}

void Barrel::processingDistortion()
{
    const int cx = m_w / 2;
    const int cy = m_h / 2;

    const double rm = sqrt(pow(cx, 2) + pow(cy, 2));

    for (int y = 0; y < m_h; ++y)
    {
        for (int x = 0; x < m_w; ++x)
        {
            const int xp = x - cx;
            const int yp = cy - y;

            const double rb = sqrt(pow(xp, 2) + pow(yp, 2));
            const double sf = rb / rm;
            const double rn = rb*(0.24*pow(sf, 4) + 0.22*pow(sf, 2) + 1);

            const int a = 0 - xp;
            const int b = 0 - yp;

            const double alpha = atan2(-a, -b);

            const int xn = sin(alpha)*rn;
            const int yn = cos(alpha)*rn;
            // TODO using matrix transformation for getting fast processing

            const int tx = xn + cx;
            const int ty = cy - yn;

            const double rg = sqrt(pow(xn, 2) + pow(yn,2));

            m_pixels[y][x].rn = rn;
            m_pixels[y][x].rg = rg;
            m_pixels[y][x].tx = tx;
            m_pixels[y][x].ty = ty;
        }
    }
}

Pixel Barrel::getPixel(int x, int y)
{
    const Pixel& p = m_pixels[y][x];
    if ( 0 <= p.tx && p.tx < m_w && 0 <= p.ty && p.ty < m_h)
    {
        Pixel r;
        r.tx = p.tx;
        r.ty = p.ty;
        return r;
    }
    else
    {
        Pixel r;
        r.tx = -1;
        r.ty = -1;
        return r;
    }
}

void  Barrel::recalculateMatrix(const cv::Mat& src, cv::Mat& dst)
{
    const auto cn = src.channels();
    for (int y = 0; y < src.rows; ++y)
    {
        for (int x = 0; x < src.cols; ++x)
        {
            const auto p = getPixel(x, y);
            const auto tx = p.tx;
            const auto ty = p.ty;

            unsigned char r = 0;
            unsigned char g = 0;
            unsigned char b = 0;
            if (p.tx != -1 && p.ty != -1)
            {
                r = src.data[ty*src.cols*cn + tx*cn + 0];
                g = src.data[ty*src.cols*cn + tx*cn + 1];
                b = src.data[ty*src.cols*cn + tx*cn + 2];
            }
            dst.data[y*src.cols*cn + x*cn + 0] = r;
            dst.data[y*src.cols*cn + x*cn + 1] = g;
            dst.data[y*src.cols*cn + x*cn + 2] = b;
        }
    }
}

#endif // end USE_OPENCV