#include "clipboarddata.h"

ClipboardData::ClipboardData(const QString &plaintext, const QString &content, const QImage &image, ContentType type) {
    this->id = -1;
    this->plaintext = plaintext;
    this->content = content;
    this->image = image;
    this->type = type;
    this->createdAt = QDate();
}

ClipboardData::ClipboardData(int id, const QString &plaintext, const QString &content, const QImage &image, ContentType type, const QDate &createdAt) {
    this->id = id;
    this->plaintext = plaintext;
    this->content = content;
    this->image = image;
    this->type = type;
    this->createdAt = createdAt;
}