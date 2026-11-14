#pragma once

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QString>
#include <QResizeEvent>
#include <QElapsedTimer>

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

private:
    QElapsedTimer _timer;
    QString _inputImagePath;
    QImage _inputImage;
    QImage _resultImage;
    Ui::MainWindow *_ui = nullptr;
};
