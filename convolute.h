#pragma once

#include <QString>
#include <QSharedPointer>

#include <vector>

namespace convolute {

/**
    Struct for holding single convolution kernel definition.
*/
struct Kernel
{
public:
    typedef std::vector<std::vector<float>> KernelMatrix;

    Kernel()
    {
        matrix.resize(3);
        for(auto& row : matrix)
        {
            row.resize(3, 0);
        }
    }

    Kernel(const KernelMatrix& m)
    {
        Q_ASSERT(m.size() == 3);
        Q_ASSERT(m[0].size() == 3);
        Q_ASSERT(m[1].size() == 3);
        Q_ASSERT(m[2].size() == 3);
        matrix = m;
    }

    /**
        Set single matrix cell.

        @param row Matrix row. Must be in range [0-2].
        @param column Matrix column. Must be in range [0-2].
        @param value Value to set.
    */
    void set(int row, int column, float value)
    {
        Q_ASSERT(row >= 0 && row < 3);
        Q_ASSERT(column >= 0 && column < 3);
        matrix[row][column] = value;
    }

    /**
        Get single matrix cell.

        @param row Matrix row. Must be in range [0-2].
        @param column Matrix column. Must be in range [0-2].
        @return Value from specified cell.
    */
    float get(int row, int column) const
    {
        Q_ASSERT(row >= 0 && row < 3);
        Q_ASSERT(column >= 0 && column < 3);
        return matrix[row][column];
    }

private:
    KernelMatrix matrix;
};

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
const Kernel& getKernel(const QString& name);

/**
    Processes input image with supplied convolution kernel.

    @param input Image to process.
    @param input Convolution kernel to use.
    @return Converted image. Ownership transfers to the caller.
*/
QSharedPointer<QImage> processImage(QSharedPointer<QImage> input, const Kernel& kernel);

}


