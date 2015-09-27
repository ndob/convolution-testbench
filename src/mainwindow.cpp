#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "convolute.h"

#include <QFileDialog>
#include <QFileInfo>
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
    inputImage(nullptr),
    outputImage(nullptr),
    inputImageLabel(nullptr),
    outputImageLabel(nullptr)
{
    ui->setupUi(this);
    setWindowTitle(tr(qPrintable(APP_NAME)));

    inputImageLabel = new QLabel;
    setupImageLabel(inputImageLabel, ui->inputLayout);

    outputImageLabel = new QLabel;
    setupImageLabel(outputImageLabel, ui->outputLayout);  

    resetLabel(inputImageLabel);
    resetLabel(outputImageLabel);

    setupKernelValidators();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupImageLabel(QLabel* imageLabel, QLayout* addTo)
{
    imageLabel->setBackgroundRole(QPalette::Midlight);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    auto scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    // Ownership of imageLabel transfers to scroller.
    scrollArea->setWidget(imageLabel);
    // Ownership of scrollArea transfers to layout.
    addTo->addWidget(scrollArea);
}

void MainWindow::resetLabel(QLabel* label)
{
    label->resize(0, 0);
    label->clear();
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

void MainWindow::showPositiveMessage(const QString& text)
{
    QMessageBox::information(this,
                         tr(qPrintable(APP_NAME)),
                         tr(qPrintable(text)),
                         QMessageBox::Ok);
}

void MainWindow::showNegativeMessage(const QString &text)
{
    QMessageBox::warning(this,
                         tr(qPrintable(APP_NAME)),
                         tr(qPrintable(text)),
                         QMessageBox::Ok);
}

void MainWindow::loadInputImage()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));

    if(fileName.size() > 0)
    {
        inputImage = QSharedPointer<QImage>::create();
        inputImage->load(fileName);
        inputImageLabel->setPixmap(QPixmap::fromImage(*inputImage));
        inputImageLabel->resize(inputImage->width(), inputImage->height());

        resetLabel(outputImageLabel);
        outputImage.clear();
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
    if(inputImage.isNull())
    {
        showNegativeMessage("Input image not selected.");
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

    outputImage = convolute::processImage(inputImage, kernel);
    outputImageLabel->setPixmap(QPixmap::fromImage(*outputImage));
    outputImageLabel->resize(inputImage->width(), inputImage->height());
}

void MainWindow::saveOutputImage()
{
    if(outputImage.isNull())
    {
        showNegativeMessage("Output image not available.");
        return;
    }

    bool result = false;
    QString targetFile = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(),tr("Image Files .png"));
    QFileInfo info(targetFile);
    if(!targetFile.isEmpty())
    {
        if(info.suffix().isEmpty() || info.suffix() != "png")
        {
            targetFile += ".png";
        }
        result = outputImage->save(targetFile);
    }

    if(result)
    {
        showPositiveMessage("Image saved successfully.");
    }
    else
    {
        showNegativeMessage("Image was not saved.");
    }
}
