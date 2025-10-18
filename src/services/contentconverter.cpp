#include "contentconverter.h"

QByteArray ContentConverter::imageToBlob(const QImage &image) {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    return byteArray;
}

QImage ContentConverter::blobToImage(const QByteArray &blob) {
    QImage image;
    image.loadFromData(blob, "PNG");
    return image;
}
