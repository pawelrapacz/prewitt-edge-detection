#pragma once

#include <QMainWindow>
#include <QImage>
#include <QString>

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
    void loadImageFromEditedPath();
    void loadImageFromDialog();
    void startEdgeDetcetion();

private:
    QImage _image;
    Ui::MainWindow *_ui = nullptr;
};
