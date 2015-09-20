#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "convolutionkerneldialog.h"
#include "convolute.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inputImage(new QImage()),
    outputImage(new QImage())
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete inputImage;
    delete outputImage;
}

void MainWindow::loadInputImage()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));

    if(fileName.size() > 0)
    {
        inputImage->load(fileName);
        ui->input->setPixmap(QPixmap::fromImage(*inputImage));
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
        ui->output->setPixmap(QPixmap::fromImage(*outputImage));
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


