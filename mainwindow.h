#pragma once

#include <QMainWindow>

#include "convolute.h"

namespace Ui {
class MainWindow;
}

class QImage;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void loadInputImage();
    void selectPredefinedKernel();
    void filterImage();
    void saveOutputImage();

private:
    void setupImageLabel(QLabel* imageLabel, QLayout* addTo);
    void setupKernelValidators();

    QScopedPointer<Ui::MainWindow> ui;
    QImage* inputImage;
    QLabel* inputImageLabel;
    QImage* outputImage;
    QLabel* outputImageLabel;
};
