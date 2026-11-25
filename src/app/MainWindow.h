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

#pragma once

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QString>
#include <QResizeEvent>
#include <QElapsedTimer>
#include <QFutureWatcher>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadImage(const QString &path);
    void loadImageFromPath();
    void loadImageFromDialog();
    void startEdgeDetcetion();
    void saveImage();

private:
    void drawImage(const QImage& image, QLabel& label);
    void resizeEvent(QResizeEvent *event) override;
    void edgeDetectionFinished();

    struct DetectResult {
        uint8_t* output;
        size_t width;
        size_t height;
        size_t bytesPerLine;
        QImage::Format fmt;
    };

private:
    QElapsedTimer _timer;
    QString _inputImagePath;
    QImage _srcImage;
    QImage _inputImage;
    QImage _resultImage;
    QFutureWatcher<DetectResult>* _watcher = nullptr;
    Ui::MainWindow *_ui = nullptr;
    bool _processing = false;
};
