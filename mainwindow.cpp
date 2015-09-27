#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "convolutionkerneldialog.h"
#include "convolute.h"

#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inputImage(new QImage()),
    inputImageLabel(new QLabel()),
    outputImage(new QImage()),
    outputImageLabel(new QLabel())
{
    ui->setupUi(this);
    setupImageLabel(inputImageLabel, ui->inputLayout);
    setupImageLabel(outputImageLabel, ui->outputLayout);
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

void MainWindow::loadInputImage()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));

    if(fileName.size() > 0)
    {
        inputImage->load(fileName);
        inputImageLabel->setPixmap(QPixmap::fromImage(*inputImage));
        inputImageLabel->resize(inputImage->width(), inputImage->height());
    }
}

void MainWindow::filterImage()
{
    if(inputImage->isNull())
    {
        QMessageBox::warning(this,
                             tr("My Application"),
                             tr("Input image not selected."),
                             QMessageBox::Ok);
        return;
    }

    auto kernelDialog = new ConvolutionKernelDialog(this);
    auto result = kernelDialog->exec();
    if(result == QDialog::Accepted)
    {
        outputImage = convolute::processImage(*inputImage, kernelDialog->convolution);
        outputImageLabel->setPixmap(QPixmap::fromImage(*outputImage));
        outputImageLabel->resize(inputImage->width(), inputImage->height());
    }
}

void MainWindow::saveOutputImage()
{
    if(outputImage->isNull())
    {
        QMessageBox::warning(this,
                             tr("My Application"),
                             tr("Output image not selected."),
                             QMessageBox::Ok);
        return;
    }

    auto targetFile = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(),tr("Image Files (*.png *.jpg *.bmp)"));
    outputImage->save(targetFile);
}


