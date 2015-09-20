#pragma once

#include <QString>
#include <QImage>

#include <vector>

namespace convolute {

/**
    Convolution kernel, which is a 3x3 matrix.
*/
typedef std::vector<std::vector<float>> Kernel;

/**
    Returns a list of names of available preconfigured
    kernels.

    @return List of kernel names.
*/
std::vector<QString> availableKernels();

/**
    Returns a kernel definition as a matrix.

    @param name Kernel name.
    @return Kernel as 3x3 matrix.
*/
const Kernel& getMatrix(const QString& name);

/**
    Processes input image with supplied convolution kernel.

    @param input Image to process.
    @param input Convolution kernel to use.
    @return Converted image. Ownership transfers to the caller.
*/
QImage* processImage(const QImage& input, const Kernel& kernel);

}


