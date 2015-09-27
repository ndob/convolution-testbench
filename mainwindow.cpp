#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QImage>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>

namespace {

const QString APP_NAME = "Convolution Testbench";
const double KERNEL_VALIDATOR_MIN_VALUE = -100.0;
const double KERNEL_VALIDATOR_MAX_VALUE = 100.0;
const int KERNEL_VALIDATOR_DECIMALS = 5;

} // unnamed namespace

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inputImage(new QImage()),
    inputImageLabel(new QLabel()),
    outputImage(new QImage()),
    outputImageLabel(new QLabel())
{
    ui->setupUi(this);
    setWindowTitle(tr(qPrintable(APP_NAME)));
    setupImageLabel(inputImageLabel, ui->inputLayout);
    setupImageLabel(outputImageLabel, ui->outputLayout);
    resetImage(inputImageLabel, inputImage);
    resetImage(outputImageLabel, outputImage);
    setupKernelValidators();
}

MainWindow::~MainWindow()
{
    delete inputImage;
    delete inputImageLabel;
    delete outputImage;
    delete outputImageLabel;
}

void MainWindow::setupImageLabel(QLabel* imageLabel, QLayout* addTo)
{
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    auto scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    addTo->addWidget(scrollArea);
}

void MainWindow::resetImage(QLabel* label, QImage* image)
{
    label->resize(0, 0);
    image->fill(QColor::fromRgb(255, 255, 255));
}

void MainWindow::setupKernelValidators()
{
    auto inputCells = findChildren<QLineEdit*>();
    for(auto cell : inputCells)
    {
        auto validator = new QDoubleValidator(KERNEL_VALIDATOR_MIN_VALUE,
                                              KERNEL_VALIDATOR_MAX_VALUE,
                                              KERNEL_VALIDATOR_DECIMALS,
                                              this);
        validator->setNotation(QDoubleValidator::StandardNotation);
        cell->setValidator(validator);
    }
}

void MainWindow::loadInputImage()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));

    if(fileName.size() > 0)
    {
        inputImage->load(fileName);
        inputImageLabel->setPixmap(QPixmap::fromImage(*inputImage));
        inputImageLabel->resize(inputImage->width(), inputImage->height());

        resetImage(outputImageLabel, outputImage);
    }
}

void MainWindow::selectPredefinedKernel()
{
    QStringList items;

    auto available = convolute::availableKernels();
    for(auto name : available)
    {
        items << tr(qPrintable(name));
    }

    bool ok;
    QString selected = QInputDialog::getItem(this, tr("Select kernel"),
                                         tr("Kernel:"), items, 0, false, &ok);
    if(ok)
    {
        auto kernel = convolute::getKernel(selected);

        auto inputCells = findChildren<QLineEdit*>();
        for(auto cell : inputCells)
        {
            int row = cell->property("matrixRow").toInt();
            int column = cell->property("matrixColumn").toInt();
            cell->setText(QString::number(kernel.get(row, column)));
        }
    }
}

void MainWindow::filterImage()
{
    if(inputImage->isNull())
    {
        QMessageBox::warning(this,
                             tr(qPrintable(APP_NAME)),
                             tr("Input image not selected."),
                             QMessageBox::Ok);
        return;
    }

    convolute::Kernel kernel;

    auto inputCells = findChildren<QLineEdit*>();
    for(auto cell : inputCells)
    {
        int row = cell->property("matrixRow").toInt();
        int column = cell->property("matrixColumn").toInt();
        kernel.set(row, column, cell->text().toFloat());
    }

    outputImage = convolute::processImage(*inputImage, kernel);
    outputImageLabel->setPixmap(QPixmap::fromImage(*outputImage));
    outputImageLabel->resize(inputImage->width(), inputImage->height());
}

void MainWindow::saveOutputImage()
{
    if(outputImage->isNull())
    {
        QMessageBox::warning(this,
                             tr(qPrintable(APP_NAME)),
                             tr("Output image not selected."),
                             QMessageBox::Ok);
        return;
    }

    auto targetFile = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(),tr("Image Files (*.png *.jpg *.bmp)"));
    outputImage->save(targetFile);
}


