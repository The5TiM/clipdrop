#include "clipboarditem.h"
#include "ui_clipboarditem.h"

ClipboardItem::ClipboardItem(QWidget *parent, QClipboard *clipboard, ClipboardData *data)
    : QWidget(parent)
    , ui(new Ui::ClipboardItem)
{
    ui->setupUi(this);

    this->clipboard = clipboard;
    this->data = data;

    updateUI();

    switch (data->getType()) {
        case ContentType::text:
        case ContentType::code:
            if (!data->getPlaintext().isEmpty()) {
                ui->content->setPlainText(data->getPlaintext());
                ui->image->hide();
            }
            break;
        case ContentType::color:
            if (!data->getPlaintext().isEmpty() && !data->getImage().isNull()) {
                ui->content->setPlainText(data->getPlaintext());
                ui->image->setPixmap(QPixmap::fromImage(data->getImage()).scaled(clipboardImageSize, clipboardImageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            break;
        case ContentType::link:
            if (!data->getPlaintext().isEmpty()) {
                ui->content->setHtml("<a href=\"" + data->getPlaintext() + "\">" + data->getPlaintext() + "</a>");
                ui->image->hide();
            }
            break;
        case ContentType::image:
            if (!data->getImage().isNull()) {
                ui->image->setPixmap(QPixmap::fromImage(data->getImage()).scaled(clipboardImageSize, clipboardImageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                ui->content->hide();
            }
            break;
        case ContentType::file:
            if (!data->getPlaintext().isEmpty()) {
                ui->content->setPlainText(data->getPlaintext());
                QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew));
                ui->image->setPixmap(icon.pixmap(clipboardImageSize, clipboardImageSize));
            }
            break;
        default:
            break;
    }
}

ClipboardItem::~ClipboardItem()
{
    delete data;
    delete ui;
}

void ClipboardItem::updateUI()
{
    if (data->getType() == ContentType::file) {
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew));
        ui->image->setPixmap(icon.pixmap(clipboardImageSize, clipboardImageSize));
    } else if (data->getType() == ContentType::code) {
        ui->content->setStyleSheet("font-family: 'Courier New'; font-size: 10pt;");
    } else {
        ui->content->setStyleSheet("font-family: 'Segoe UI'; font-size: 10pt;");
    }
}

void ClipboardItem::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::PaletteChange) {
        updateUI();
    }
}

void ClipboardItem::on_remove_clicked()
{
    emit removeRequested(data->getId());
}

void ClipboardItem::on_addToClipboard_clicked()
{
    if (!clipboard || !data) {
        return;
    }

    QMimeData *mimeData = nullptr;

    switch (data->getType()) {
        case ContentType::text:
        case ContentType::link:
        case ContentType::code:
        case ContentType::color: {
            if (!data->getPlaintext().isEmpty()) {
                mimeData = new QMimeData();
                mimeData->setText(data->getPlaintext());
                if (!data->getContent().isEmpty()) {
                    mimeData->setHtml(data->getContent());
                }
                clipboard->setMimeData(mimeData);
            }
            break;
        }
        case ContentType::image: {
            if (!data->getImage().isNull()) {
                mimeData = new QMimeData();
                mimeData->setImageData(data->getImage());
                clipboard->setMimeData(mimeData);
            }
            break;
        }
        case ContentType::file: {
            QList<QUrl> urlList;
            const QStringList urlStrings = data->getContent().split('\n');
            for (const QString &urlString : urlStrings) {
                urlList.append(QUrl(urlString));
            }

            if (!urlList.isEmpty()) {
                mimeData = new QMimeData();
                mimeData->setUrls(urlList);
                clipboard->setMimeData(mimeData);
            }
            break;
        }
        default:
            break;

    }
}
