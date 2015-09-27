#pragma once

#include <QMainWindow>

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
    void filterImage();
    void saveOutputImage();

private:
    void setupImageLabel(QLabel* imageLabel, QLayout* addTo);

    QScopedPointer<Ui::MainWindow> ui;
    QImage* inputImage;
    QLabel* inputImageLabel;
    QImage* outputImage;
    QLabel* outputImageLabel;
};
