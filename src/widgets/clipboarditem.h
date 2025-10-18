#ifndef CLIPBOARDITEM_H
#define CLIPBOARDITEM_H

#include <QWidget>
#include <QClipboard>
#include <QMainWindow>
#include <QMimeData>

#include <clipboarddata.h>
#include <contenttype.h>

namespace Ui {
class ClipboardItem;
}

class ClipboardItem : public QWidget
{
    Q_OBJECT

public:
    explicit ClipboardItem(QWidget *parent = nullptr, QClipboard *clipboard = nullptr, ClipboardData *data = nullptr);
    ~ClipboardItem();
    int getId() const { return id; }

signals:
    void removeRequested(int id);

private slots:
    void on_remove_clicked();
    void on_addToClipboard_clicked();

private:
    Ui::ClipboardItem *ui;
    QClipboard *clipboard;
    ClipboardData *data;

    void changeEvent(QEvent *event);
    void updateUI();

    const int clipboardImageSize = 60;

    int id;
    ContentType type;
    QString plaintext;
    QString content;
    QImage image;
};

#endif // CLIPBOARDITEM_H
