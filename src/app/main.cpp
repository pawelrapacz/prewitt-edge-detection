#include "MainWindow.h"

#include <QApplication>
#include <QImageReader>
#include <qimagereader.h>

int main(int argc, char** argv) {
    QImageReader::setAllocationLimit(1024); // 1024 MB

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}