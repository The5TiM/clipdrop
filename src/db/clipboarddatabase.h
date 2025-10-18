#ifndef CLIPBOARDDATABASE_H
#define CLIPBOARDDATABASE_H

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDate>

#include <contenttype.h>
#include <contentconverter.h>
#include <clipboarddata.h>

class ClipboardDatabase
{
public:
    ClipboardDatabase(const QString& path);
    ~ClipboardDatabase();
    QSqlDatabase getDatabase() { return db; }

    int addClipboardData(ClipboardData *data);
    void removeClipboardData(int id);
    QList<ClipboardData> getClipboardData(const QString &searchText = "", ContentType type = ContentType::all);
    void clearAllClipboardData();

private:
    QSqlDatabase db;
};

#endif // CLIPBOARDDATABASE_H
