#ifndef CLIPBOARDDATA_H
#define CLIPBOARDDATA_H

#include <QString>
#include <QImage>
#include <QDate>

#include <contenttype.h>

class ClipboardData
{
public:
    ClipboardData(const QString &plaintext, const QString &content, const QImage &image, ContentType type);
    ClipboardData(int id, const QString &plaintext, const QString &content, const QImage &image, ContentType type, const QDate &createdAt);

    int getId() const { return id; }
    ContentType getType() const { return type; }
    QString getPlaintext() const { return plaintext; }
    QString getContent() const { return content; }
    QImage getImage() const { return image; }
    QDate getCreatedAt() const { return createdAt; }

    void setId(int newId) { id = newId; }
    
private:
    int id;
    QString plaintext;
    QString content;
    QImage image;
    ContentType type;
    QDate createdAt;
};

#endif // CLIPBOARDDATA_H
