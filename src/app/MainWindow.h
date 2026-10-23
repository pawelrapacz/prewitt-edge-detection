#pragma once

#include <QMainWindow>
#include <QImage>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void chooseFile();
    void startEdgeDetcetion();

private:
    QImage _image;
    QPixmap _pixmap;
    Ui::MainWindow *_ui = nullptr;
};
