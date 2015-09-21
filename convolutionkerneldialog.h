#pragma once

#include <QDialog>
#include "convolute.h"

namespace Ui {
class ConvolutionKernelDialog;
}

class ConvolutionKernelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConvolutionKernelDialog(QWidget *parent = 0);
    ~ConvolutionKernelDialog();

    convolute::Kernel convolution;

public slots:
    void accept();
    void selectPredefinedKernel();

private:
    QScopedPointer<Ui::ConvolutionKernelDialog> ui;
};
