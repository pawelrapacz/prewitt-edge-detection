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

#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>

class ImageLabel : public QLabel {
    Q_OBJECT

public:
    ImageLabel(QWidget *parent = nullptr);
    ~ImageLabel();

signals:
    void imageDropped(const QString &path);

private:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};