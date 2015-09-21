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

    static const double MIN_VALUE = -100.0;
    static const double MAX_VALUE = 100.0;
    static const int DECIMALS = 5;

    auto inputCells = findChildren<QLineEdit*>();
    for(auto cell : inputCells)
    {
        auto validator = new QDoubleValidator(MIN_VALUE, MAX_VALUE, DECIMALS, this);
        validator->setNotation(QDoubleValidator::StandardNotation);
        cell->setValidator(validator);
    }
}

ConvolutionKernelDialog::~ConvolutionKernelDialog()
{
}

void ConvolutionKernelDialog::accept()
{
    QDialog::accept();
    convolution.clear();
    convolution.resize(3);
    for(auto& row : convolution)
    {
        row.resize(3, 0);
    }

    auto inputCells = findChildren<QLineEdit*>();
    for(auto cell : inputCells)
    {
        int row = cell->property("matrixRow").toInt();
        int column = cell->property("matrixColumn").toInt();
        convolution.at(row).at(column) = cell->text().toFloat();
    }
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

        auto inputCells = findChildren<QLineEdit*>();
        for(auto cell : inputCells)
        {
            int row = cell->property("matrixRow").toInt();
            int column = cell->property("matrixColumn").toInt();
            cell->setText(QString::number(matrix[row][column]));
        }
    }
}
