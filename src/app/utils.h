#pragma once

#include <QFileInfo>
#include <QMimeData>
#include <QUrl>

inline bool isAllowedImageFile(const QFileInfo &fi) {
    auto ext = fi.suffix().toLower();

    if (ext == "bmp" || ext == "jpg" || ext == "jpeg" || ext == "png")
        return true;
    else
        return false;
}

inline bool isAllowedImageUrl(const QMimeData *mimeData) {
    return
        mimeData->hasUrls() &&
        mimeData->urls().size() == 1 &&
        mimeData->urls().first().isLocalFile() &&
        isAllowedImageFile(QFileInfo(mimeData->urls().first().toLocalFile()));
}