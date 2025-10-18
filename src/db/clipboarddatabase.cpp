#include "clipboarddatabase.h"

ClipboardDatabase::ClipboardDatabase(const QString& path) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    db.open();
}

ClipboardDatabase::~ClipboardDatabase() {
    if (db.isOpen()) {
        db.close();
    }
}

int ClipboardDatabase::addClipboardData(ClipboardData *data) {
    QByteArray imageBlob = ContentConverter::imageToBlob(data->getImage());
    int typeId = static_cast<int>(data->getType());

    QSqlQuery query(db);
    query.prepare("INSERT INTO clipboard (plaintext, content, image, type_id) VALUES (:plaintext, :content, :image, :type_id)");
    query.bindValue(":plaintext", data->getPlaintext());
    query.bindValue(":content", data->getContent());
    query.bindValue(":image", imageBlob);
    query.bindValue(":type_id", typeId);

    if (!query.exec()) {
        return -1;
    }

    return query.lastInsertId().toInt();
}

void ClipboardDatabase::removeClipboardData(int id) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM clipboard WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
}

QList<ClipboardData> ClipboardDatabase::getClipboardData(const QString &searchText, ContentType type) {
    QList<ClipboardData> dataList;
    QSqlQuery query(db);

    if (searchText.isEmpty() && type == ContentType::all) {
        query.prepare("SELECT id, plaintext, content, image, type_id, created_at FROM clipboard ORDER BY created_at DESC");
    } else if (type == ContentType::all) {
        query.prepare("SELECT id, plaintext, content, image, type_id, created_at FROM clipboard WHERE LOWER(plaintext) LIKE LOWER(:searchText) ORDER BY created_at DESC");
        query.bindValue(":searchText", "%" + searchText + "%");
    } else if (searchText.isEmpty()) {
        int typeId = static_cast<int>(type);
        query.prepare("SELECT id, plaintext, content, image, type_id, created_at FROM clipboard WHERE type_id = :typeId ORDER BY created_at DESC");
        query.bindValue(":typeId", typeId);
    } else {
        int typeId = static_cast<int>(type);
        query.prepare("SELECT id, plaintext, content, image, type_id, created_at FROM clipboard WHERE LOWER(plaintext) LIKE LOWER(:searchText) AND type_id = :typeId ORDER BY created_at DESC");
        query.bindValue(":searchText", "%" + searchText + "%");
        query.bindValue(":typeId", typeId);
    }

    query.exec();
    
    while (query.next()) {
        int id = query.value(0).toInt();
        QString plaintext = query.value(1).toString();
        QString content = query.value(2).toString();
        QByteArray imageBlob = query.value(3).toByteArray();
        int typeId = query.value(4).toInt();
        QDate createdAt = query.value(5).toDate();

        QImage image = ContentConverter::blobToImage(imageBlob);
        ContentType type = static_cast<ContentType>(typeId);

        ClipboardData data(id, plaintext, content, image, type, createdAt);
        dataList.append(data);
    }
    return dataList;
}

void ClipboardDatabase::clearAllClipboardData() {
    QSqlQuery query(db);
    query.exec("DELETE FROM clipboard");
}
