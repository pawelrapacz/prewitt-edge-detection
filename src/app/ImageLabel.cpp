#include "ImageLabel.h"

#include "utils.h"

#include <QMimeData>
#include <QPixmap>

ImageLabel::ImageLabel(QWidget *parent)
    : QLabel(parent)
{
    setAcceptDrops(true);
}

ImageLabel::~ImageLabel() {
}


void ImageLabel::dragEnterEvent(QDragEnterEvent *event)
{
    if (isAllowedImageUrl(event->mimeData())) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void ImageLabel::dropEvent(QDropEvent *event)
{
    if (isAllowedImageUrl(event->mimeData())) {
        event->acceptProposedAction();
        QString path = event->mimeData()->urls().first().toLocalFile();

        emit imageDropped(path);
    } else {
        event->ignore();
    }
}