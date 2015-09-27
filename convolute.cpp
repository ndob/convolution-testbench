#include "convolute.h"

#include <algorithm>
#include <map>

namespace {

using convolute::Kernel;

const Kernel MATRIX_BLUR({
    {0.06250f,  0.125f,     0.0625f},
    {0.125f,    0.25f,      0.125f},
    {0.0625f,   0.125f,     0.0625f}
});

const Kernel MATRIX_EMBOSS({
    {-2.f,      -1.f,       0.f},
    {-1.f,      1.f,        1.f},
    {0.f,       1.f,        2.f}
});

const Kernel MATRIX_SHARPEN({
    {0.f,      -1.f,        0.f},
    {-1.f,     5.f,        -1.f},
    {0.f,      -1.f,        0.f}
});

const Kernel MATRIX_IDENTITY({
    {0.f,       0.f,        0.f},
    {0.f,       1.f,        0.f},
    {0.f,       0.f,        0.f}
});

const std::map<QString, const Kernel*> stringToKernel {
    {"blur", &MATRIX_BLUR},
    {"emboss", &MATRIX_EMBOSS},
    {"sharpen", &MATRIX_SHARPEN},
    {"identity", &MATRIX_IDENTITY}
};

float clamp0_255(float val)
{
    return std::max(0.f, std::min(val, 255.0f));
}

QRgb convMatrix(const QImage& image, int x, int y, const Kernel& kernel)
{
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
    float a = 0.f;

    const int xMax = image.width() - 1;
    const int yMax = image.height() - 1;

    for(int i = x - 1, im = 0; i <= x + 1; ++i, ++im)
    {
        if(i < 0 || i > xMax) continue;
        for(int j = y - 1, jm = 0; j <= y + 1; ++j, ++jm)
        {
            if(j < 0 || j > yMax) continue;
            QRgb c = image.pixel(i, j);
            float kernelValue = kernel.get(im, jm);
            r += (float) qRed(c) * kernelValue;
            g += (float) qGreen(c) * kernelValue;
            b += (float) qBlue(c) * kernelValue;
            a += (float) qAlpha(c) * kernelValue;
        }
    }
    return qRgba(clamp0_255(r), clamp0_255(g), clamp0_255(b), clamp0_255(a));
}

} // unnamed namespace

namespace convolute {

std::vector<QString> availableKernels()
{
    std::vector<QString> ret;
    for(const auto& kv : stringToKernel)
    {
        ret.push_back(kv.first);
    }
    return ret;
}

const Kernel& getKernel(const QString& name)
{
    auto it = stringToKernel.find(name);
    if(it != stringToKernel.end())
    {
        return *(it->second);
    }
    return MATRIX_IDENTITY;
}

QImage* processImage(const QImage& input, const Kernel& kernel)
{
    auto output = new QImage(input.width(), input.height(), QImage::Format_ARGB32);

    for(int x = 0; x < input.width(); ++x)
    {
        for(int y = 0; y < input.height(); ++y)
        {
            output->setPixel(x,y, convMatrix(input, x, y, kernel));
        }
    }

    return output;
}

} // convolute namespace
