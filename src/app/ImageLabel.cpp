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