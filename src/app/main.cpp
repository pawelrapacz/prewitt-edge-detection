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

#include <QApplication>
#include <QImageReader>

int main(int argc, char** argv) {
    QImageReader::setAllocationLimit(1024); // 1024 MB

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}