#include "MainWindow.h"
#include "ImageLabel.h"
#include "ui_MainWindow.h"
#include "utils.h"

#include <cimpl.h>
#include <vector>

#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QFileInfo>
#include <QPushButton>
#include <QString>
#include <QMessageBox>
#include <QResizeEvent>
#include <QElapsedTimer>
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

    _ui->imageSize->setText(QString("Image size: %1x%2").arg(_image.width()).arg(_image.height()));
    _ui->pathEdit->setText(path);
    drawImage();
}


void MainWindow::loadImageFromEditedPath() {
    QFileInfo fileInfo(_ui->pathEdit->text());

    if (not fileInfo.isFile()) {
        QMessageBox::warning(this, "File not found", fileInfo.filePath() + " is not a file or does not exist.");
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

    QImage gray = _image.convertToFormat(QImage::Format_Grayscale8);

    std::vector<uint8_t> input(
        static_cast<size_t>(width) * height
    );

    for (int y = 0; y < height; ++y) {
        std::memcpy(
            input.data() + static_cast<size_t>(y) * width,
            gray.constScanLine(y),
            static_cast<size_t>(width)
        );
    }

    uint8_t* output = nullptr;
    if (_ui->implementationPicker->currentIndex() == 0) { // asm implementation
        qWarning() << "call asm implementation!"; // TODO: call asm implementation
        return;
        
    } else { // c implementation
        _timer.start();
        output = cDetectEdges(width, height, input.data());
        auto timeNs = _timer.nsecsElapsed();
        _ui->time->setText(QString("%1")
        .arg(timeNs / 1e6, 0, 'f', 3)
);
    }


    if (output) {
        _image = QImage(
            output,
            width,
            height,
            width,
            QImage::Format_Grayscale8
        );

        drawImage();
    }
}


void MainWindow::drawImage() {
    if (_image.isNull()) return;

    _ui->imageLabel->setPixmap(QPixmap::fromImage(_image).scaled(
        _ui->imageLabel->size(),
        Qt::KeepAspectRatio,
        Qt::FastTransformation
    ));
}


void MainWindow::resizeEvent(QResizeEvent *event) {
    drawImage();
}