#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

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
    QScopedPointer<Ui::MainWindow> ui;
    QImage* inputImage;
    QImage* outputImage;
};
