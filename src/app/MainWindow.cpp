#include "MainWindow.h"
#include "ImageLabel.h"
#include "ui_MainWindow.h"
#include "utils.h"

#include <cimpl.h>

#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QFileInfo>
#include <QPushButton>
#include <QString>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}, _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    connect(_ui->imageLabel, &ImageLabel::imageDropped, this, &MainWindow::loadImage);
    connect(_ui->pathEdit, &QLineEdit::editingFinished, this, &MainWindow::loadImageFromEditedPath);
    connect(_ui->chooseImageBtn, &QPushButton::clicked, this, &MainWindow::loadImageFromDialog);
    connect(_ui->startBtn, &QPushButton::clicked, this, &MainWindow::startEdgeDetcetion);
}

MainWindow::~MainWindow() {
    delete _ui;
}


void MainWindow::loadImage(const QString &path) {
    if (path.isEmpty())
        return;

    _image = QImage(path);

    _ui->pathEdit->setText(path);
    _ui->imageLabel->setPixmap(QPixmap::fromImage(_image).scaled(
        _ui->imageLabel->size(),
        Qt::KeepAspectRatio
    ));
}

void MainWindow::loadImageFromEditedPath() {
    QFileInfo fileInfo(_ui->pathEdit->text());

    if (not fileInfo.isFile()) {
        QMessageBox::warning(this, "File not found", fileInfo.path() + " is not a file or does not exist.");
        return;
    }

    if (not isAllowedImageFile(fileInfo)) {
        QMessageBox::warning(this, "Unsopported file format", "File format is not allowed. Only png, jpg and bmp files are allowed.");
        return;
    }

    loadImage(_ui->pathEdit->text());
}

void MainWindow::loadImageFromDialog() {
    QString path = QFileDialog::getOpenFileName(this, "Choose file", "~/","(*.png *.jpg *.jpeg *.bmp)");
    loadImage(path);
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