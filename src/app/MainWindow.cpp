#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <cimpl.h>

#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QMessageBox>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}, _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    connect(_ui->chooseFileBtn, &QPushButton::clicked, this, &MainWindow::chooseFile);
    connect(_ui->startBtn, &QPushButton::clicked, this, &MainWindow::startEdgeDetcetion);
}

MainWindow::~MainWindow() {
    delete _ui;
}


void MainWindow::chooseFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Choose file", "~/","(*.png *.jpg *.bmp)");

    if (fileName.isEmpty())
        return;

    _image = QImage(fileName);
    _pixmap.convertFromImage(_image);
    _ui->chooseFileBtn->setText(fileName);
    _ui->imageBox->setPixmap(_pixmap);
}

void MainWindow::startEdgeDetcetion() {
    if (_image.isNull()) {
        QMessageBox::information(this, "Choose a file", "To start edge detection first choose a file.");
        return;
    }

    int threads = _ui->threadCountBox->value();
    int width = _image.width();
    int height = _image.height();

    QString impl = _ui->implementationPicker->currentText();
    if (impl == "C++ implementation")
        cimplDetectEdges(nullptr, width, height, threads);
    else // asm
        qWarning() << "call asm implementation!"; // TODO: call asm implementation
}