#pragma once

#include <QMainWindow>
#include <QScopedPointer>
#include <QSharedPointer>

namespace Ui {
class MainWindow;
}

class QImage;
class QLabel;
class QLayout;

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
    void resetLabel(QLabel* label);
    void setupKernelValidators();
    void showPositiveMessage(const QString& text);
    void showNegativeMessage(const QString& text);

    QScopedPointer<Ui::MainWindow> ui;
    QSharedPointer<QImage> inputImage;
    QSharedPointer<QImage> outputImage;
    QLabel* inputImageLabel;
    QLabel* outputImageLabel;
};
