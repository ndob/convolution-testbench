#include "convolutionkerneldialog.h"
#include "ui_convolutionkerneldialog.h"

#include "convolute.h"

#include <QDialog>
#include <QInputDialog>

ConvolutionKernelDialog::ConvolutionKernelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConvolutionKernelDialog)
{
    setModal(true);
    ui->setupUi(this);
}

ConvolutionKernelDialog::~ConvolutionKernelDialog()
{
}

void ConvolutionKernelDialog::selectPredefinedKernel()
{
    QStringList items;

    auto available = convolute::availableKernels();
    for(auto name : available)
    {
        items << tr(qPrintable(name));
    }

    bool ok;
    QString selected = QInputDialog::getItem(this, tr("Select predefined kernel"),
                                         tr("Kernel:"), items, 0, false, &ok);
    if(ok)
    {
        auto matrix = convolute::getMatrix(selected);

        ui->k00->setText(QString::number(matrix[0][0]));
        ui->k01->setText(QString::number(matrix[0][1]));
        ui->k02->setText(QString::number(matrix[0][2]));

        ui->k10->setText(QString::number(matrix[1][0]));
        ui->k11->setText(QString::number(matrix[1][1]));
        ui->k12->setText(QString::number(matrix[1][2]));

        ui->k20->setText(QString::number(matrix[2][0]));
        ui->k21->setText(QString::number(matrix[2][1]));
        ui->k22->setText(QString::number(matrix[2][2]));

        convolution = matrix;
    }
}
