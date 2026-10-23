#pragma once

#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <qevent.h>

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