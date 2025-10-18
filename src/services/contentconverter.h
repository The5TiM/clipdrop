#ifndef CONTENTCONVERTER_H
#define CONTENTCONVERTER_H

#include <QImage>
#include <QByteArray>
#include <QBuffer>

class ContentConverter
{
public:
    static QByteArray imageToBlob(const QImage &image);
    static QImage blobToImage(const QByteArray &blob);
};

#endif // CONTENTCONVERTER_H
