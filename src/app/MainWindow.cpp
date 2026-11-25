/*
 * Project: Prewitt edge detection
 * 
 * The Prewitt algorithm detects edges in images by calculating
 * intensity changes in horizontal and vertical directions using
 * two convolution kernels.
 * 
 * 2026/2027 sem. 5
 * Author: Paweł Rapacz
 * 
 * Version: 1.0
 * 
 * Version history:
 * 1.0 - Initial version of the application.
 *      Implemented Prewitt edge detection in C and x86-64 ASM.
 *      Created GUI app and benchmark with core functionality.
 */

#include "MainWindow.h"
#include "ImageLabel.h"
#include "ui_MainWindow.h"
#include "utils.h"

#include <core.h>
#include <asmimpl.h>
#include <cimpl.h>

#include <QLabel>
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QFileInfo>
#include <QPushButton>
#include <QString>
#include <QMessageBox>
#include <QSpinBox>
#include <QResizeEvent>
#include <QElapsedTimer>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}, _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    connect(_ui->inputImageLabel, &ImageLabel::imageDropped, this, &MainWindow::loadImage);
    connect(_ui->pathEdit, &QLineEdit::editingFinished, this, &MainWindow::loadImageFromPath);
    connect(_ui->chooseImageBtn, &QPushButton::clicked, this, &MainWindow::loadImageFromDialog);
    connect(_ui->startBtn, &QPushButton::clicked, this, &MainWindow::startEdgeDetcetion);
    connect(_ui->saveBtn, &QPushButton::clicked, this, &MainWindow::saveImage);

    connect(_ui->amountSlider, &QSlider::valueChanged, this, [this] (int val) {
        _ui->amountSpinBox->setValue(val / 1000.0);
    });

    connect(_ui->amountSpinBox, &QDoubleSpinBox::valueChanged, this, [this] (double val) {
        _ui->amountSlider->setValue(val * 1000);
    });
}

MainWindow::~MainWindow() {
    delete _ui;
}

void MainWindow::loadImage(const QString &path) {
    if (path.isEmpty())
        return;

    _inputImage = QImage(path);
    _inputImage.convertTo(QImage::Format_RGB888);
    
    if (_inputImage.isNull()) {
        QMessageBox::warning(this, "Image not loaded", "Failed to load image:\n" + path);
        return;
    }
    
    _inputImagePath = path;
    _ui->imageSize->setText(QString("Image size: %1x%2").arg(_inputImage.width()).arg(_inputImage.height()));
    _ui->pathEdit->setText(_inputImagePath);
    drawImage(_inputImage, *(_ui->inputImageLabel));
}


void MainWindow::loadImageFromPath() {
    QFileInfo fileInfo(_ui->pathEdit->text());

    if (not fileInfo.isFile()) {
        QMessageBox::warning(this, "File not found", "\"" + fileInfo.filePath() + "\" is not a file or does not exist.");
        return;
    }

    if (not isAllowedImageFile(fileInfo)) {
        QMessageBox::warning(this, "Unsopported file format", "File format is not allowed. Only png, jpg and bmp files are allowed.");
        return;
    }

    loadImage(fileInfo.filePath());
}


void MainWindow::loadImageFromDialog() {
    QString path = QFileDialog::getOpenFileName(this, "Choose file", QString(),"(*.png *.jpg *.jpeg *.bmp)");
    loadImage(path);
}


void MainWindow::startEdgeDetcetion() {
    if (_processing)
        return;

    if (_inputImage.isNull()) {
        QMessageBox::information(this, "Image not loaded", "To start edge detection first choose a file.");
        return;
    }
    
    detect_edges_worker worker;
    if (_ui->implementationPicker->currentIndex() == 0) // asm scalar
        worker = asmDetectEdgesWorker;
    else if (_ui->implementationPicker->currentIndex() == 1) // asm SIMD
        worker = asmDetectEdgesSIMDWorker;
    else // c implementation
        worker = cDetectEdgesWorker;
        
    const int threads = _ui->threadCountBox->value();
    if (threads < 1 || threads > 64) {
        QMessageBox::warning(this, "Invalid value", "Thread count can be set from 1 to 64.");
        return;
    }

    const double amount = _ui->amountSpinBox->value();
    if (amount < 1. || amount > 10.) {
        QMessageBox::warning(this, "Invalid value", "Amount can be set from 1 to 10.");
        return;
    }

    bool isRGB = _ui->rgbCheckBox->isChecked();
    QImage::Format qfmt = isRGB ? QImage::Format_RGB888 : QImage::Format_Grayscale8;
    Format fmt = isRGB ? RGB : GRAY;
    
    _processing = true;
    _srcImage = _inputImage.convertToFormat(qfmt);
    _timer.start();

    QFuture<DetectResult> future = QtConcurrent::run([=, this] -> DetectResult {
        DetectResult res;
        res.width = _srcImage.width();
        res.height = _srcImage.height();
        res.fmt = qfmt;
        res.output = detectEdgesThreads(res.width, res.height, fmt, this->_srcImage.constBits(),
                                        this->_srcImage.bytesPerLine(), amount, &res.bytesPerLine, worker, threads);
        return res;
    });


    _watcher = new QFutureWatcher<DetectResult>(this);
    connect(_watcher, &QFutureWatcher<uint8_t*>::finished,
            this, &MainWindow::edgeDetectionFinished);
    _watcher->setFuture(future);
}

void MainWindow::edgeDetectionFinished() {
    auto timeNs = _timer.nsecsElapsed();
    _ui->time->setText(
        QString("%1").arg(timeNs / 1e6, 0, 'f', 3)
    );

    _srcImage = {};
    auto res = _watcher->result();
    _watcher->deleteLater();
    _watcher = nullptr;
    _processing = false;
    _ui->startBtn->setEnabled(true);

    _resultImage = QImage(
        res.output,
        res.width,
        res.height,
        res.bytesPerLine,
        res.fmt,
        imageBufferFree,
        res.output
    );

    if (_resultImage.isNull()) {
        imageBufferFree(res.output); // free if image construction failed
        QMessageBox::critical(this, "Image not processed", "An error occured.\nFailed to process the image.");
    }

    drawImage(_resultImage, *(_ui->resultImageLabel));
}

void MainWindow::saveImage() {
    if (_resultImage.isNull()) {
        QMessageBox::information(this, "Image not saved", "No image to save!");
        return;
    }

    QString path = QFileDialog::getSaveFileName(this, "Provide a file name to save image", "image.jpg", "(*.png *.jpg *.jpeg *.bmp)");

    if (path.isEmpty())
        return;

    if (!_resultImage.save(path))
        QMessageBox::warning(this, "Image not saved", "Could not save file: " + path);
}

void MainWindow::drawImage(const QImage& image, QLabel& label) {
    if (!image.isNull()) {
        label.setPixmap(QPixmap::fromImage(image).scaled(
            label.size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        ));
    }
}


void MainWindow::resizeEvent(QResizeEvent *event) {
    drawImage(_inputImage, *(_ui->inputImageLabel));
    drawImage(_resultImage, *(_ui->resultImageLabel));
}