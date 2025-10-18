#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initialize();
}

MainWindow::~MainWindow()
{
    delete toggleShortcut;
    delete db;
    delete ui;
}

void MainWindow::initialize()
{
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::SubWindow);
    setFixedSize(size());
    makeRoundedCorners(16);
    moveWindowToCursorPosition();

    createShowAndHideWindowShortcut();

    startClipboardMonitoring();

    initializeDatabase();
    loadClipboardItems();
}

void MainWindow::makeRoundedCorners(int r)
{
    QBitmap bitmap(size());
    bitmap.fill(Qt::color0);

    QPainter painter(&bitmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::color1);
    painter.drawRoundedRect(rect(), r, r);

    setMask(bitmap);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    currentPosition = event->globalPosition().toPoint();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
        return;

    newPosition = event->globalPosition().toPoint();
    QPoint positionDifference = newPosition - currentPosition;
    this->move(x() + positionDifference.x(), y() + positionDifference.y());
    currentPosition = newPosition;
}

void MainWindow::createShowAndHideWindowShortcut()
{
    toggleShortcut = new QHotkey(QKeySequence("Ctrl+Space"), true, this);

    QObject::connect(toggleShortcut, &QHotkey::activated, this, [this]() {
        if (this->isVisible())
        {
            this->hide();
        }
        else
        {
            moveWindowToCursorPosition();

            this->show();
            this->raise();
            this->activateWindow();
        }
    });
}

void MainWindow::moveWindowToCursorPosition()
{
    QPoint cursorPos = QCursor::pos();
    this->move(cursorPos.x(), cursorPos.y());
}

void MainWindow::startClipboardMonitoring()
{
    clipboard = QApplication::clipboard();
    QObject::connect(clipboard, &QClipboard::dataChanged, this, [this]() {

        if (this->isVisible()) return;

        const QMimeData *mimeData = clipboard->mimeData();

        if (!mimeData) return;

        QString currentCopiedContent;

        if (mimeData->hasImage()) {
            QImage image = qvariant_cast<QImage>(mimeData->imageData());
            currentCopiedContent = "[Image] " + QString::number(image.width()) + "x" + QString::number(image.height());
        } else if (mimeData->hasUrls()) {
            const QList<QUrl> urlList = mimeData->urls();
            QString urls = "";
            for (const QUrl &url : urlList) {
                urls += url.toString();
            }
            currentCopiedContent = urls;
        } else if (mimeData->hasText()) {
            currentCopiedContent = mimeData->text();
        }

        if (currentCopiedContent == lastCopiedContent || currentCopiedContent.isEmpty()) {
            return;
        }

        lastCopiedContent = currentCopiedContent;
        addNewClipboardItem(mimeData);
    });
}

void MainWindow::initializeDatabase()
{
    const QString path = "clipdrop.db";
    db = new ClipboardDatabase(path);
}

void MainWindow::loadClipboardItems()
{
    clearClipboardHistory();

    const QList<ClipboardData> dataList = db->getClipboardData(searchText, typeSelected);

    QDate today = QDate::currentDate();
    QDate yesterday = today.addDays(-1);
    QDate weekAgo = today.addDays(-7);
    QDate monthAgo = today.addMonths(-1);

    for (const ClipboardData &data : dataList) {
        QDate createdAt = data.getCreatedAt();

        if (createdAt == today) {
            addNewClipboardItem(data, ui->todayLayout);
        } else if (createdAt == yesterday) {
            addNewClipboardItem(data, ui->yesterdayLayout);
        } else if (createdAt > weekAgo) {
            addNewClipboardItem(data, ui->weekLayout);
        } else if (createdAt > monthAgo) {
            addNewClipboardItem(data, ui->monthLayout);
        } else {
            addNewClipboardItem(data, ui->longTimeAgoLayout);
        }
    }
}

void MainWindow::addNewClipboardItem(const ClipboardData &data, QVBoxLayout *layout)
{
    if (!layout) return;

    ClipboardData *copiedData = new ClipboardData(data);

    ClipboardItem *item = new ClipboardItem(this, clipboard, copiedData);
    
    QLabel* label = getLabelInLayout(layout);
    if (label && label->isHidden()) label->show();

    layout->addWidget(item);
    handleItemRemoval(item, layout);
}

void MainWindow::addNewClipboardItem(const QMimeData *mimeData)
{
    ClipboardItem *item = nullptr;
    ClipboardData *data = nullptr;
    
    if (mimeData->hasImage()) {
        QImage image = qvariant_cast<QImage>(mimeData->imageData());
        QString plaintext = "[Image] " + QString::number(image.width()) + "x" + QString::number(image.height());
        data = new ClipboardData(plaintext, "", image, ContentType::image);
        item = new ClipboardItem(this, clipboard, data);
    } else if (mimeData->hasUrls()) {
        const QList<QUrl> urlList = mimeData->urls();
        
        QString urlsPlaintext, urls = "";
        for (const QUrl &url : urlList) {
            urlsPlaintext += "- " + url.fileName() + "\n";
            urls += url.toString() + "\n";
        }
        data = new ClipboardData(urlsPlaintext, urls, QImage(), ContentType::file);
        item = new ClipboardItem(this, clipboard, data);
    } else if (mimeData->hasText()) {
        QString text = mimeData->text();
        QString html = text;
        QImage image;

        ContentType type = ContentAnalyzer::analyzeContent(text);

        if (type == ContentType::color) {
            QString colorText = text;

            QColor color(colorText);
            if (!color.isValid()) {
                colorText = "#" + text;
                color = QColor(colorText);
            }

            image = QImage(60, 60, QImage::Format_ARGB32);
            image.fill(color);
        }

        if (mimeData->hasHtml()) {
            html = mimeData->html();
        }
        data = new ClipboardData(text, html, image, type);
        item = new ClipboardItem(this, clipboard, data);
    }

    if (!item || !data) return;

    int newId = db->addClipboardData(data);

    if (newId == -1) {
        delete item;
        delete data;
        return;
    }

    data->setId(newId);

    if (ui->todayLabel->isHidden()) ui->todayLabel->show();
    handleItemRemoval(item, ui->todayLayout);
    ui->todayLayout->insertWidget(1, item);

    if (typeSelected != ContentType::all || !searchText.isEmpty()) {
        loadClipboardItems();
    }
}

void MainWindow::handleItemRemoval(ClipboardItem *item, QVBoxLayout *layout)
{
    QObject::connect(item, &ClipboardItem::removeRequested, this, [this, item, layout](int id) {
        lastCopiedContent = "";
        db->removeClipboardData(id);
        item->deleteLater();
        if (layout) {
            QLabel* label = getLabelInLayout(layout);
            if (label && layout->count() <= 2) label->hide();
        }
    });
}

QLabel* MainWindow::getLabelInLayout(QVBoxLayout *layout)
{
    if (!layout) return nullptr;

    if (layout == ui->todayLayout) return ui->todayLabel;
    if (layout == ui->yesterdayLayout) return ui->yesterdayLabel;
    if (layout == ui->weekLayout) return ui->weekLabel;
    if (layout == ui->monthLayout) return ui->monthLabel;
    if (layout == ui->longTimeAgoLayout) return ui->longTimeAgoLabel;
    
    return nullptr;
}

void MainWindow::clearLayout(QVBoxLayout *layout, QLabel *label)
{
    if (!layout || !label) return;

    label->hide();

    QLayoutItem *child;
    while ((child = layout->takeAt(1)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }
}

void MainWindow::clearClipboardHistory()
{
    clearLayout(ui->todayLayout, ui->todayLabel);
    clearLayout(ui->yesterdayLayout, ui->yesterdayLabel);
    clearLayout(ui->weekLayout, ui->weekLabel);
    clearLayout(ui->monthLayout, ui->monthLabel);
    clearLayout(ui->longTimeAgoLayout, ui->longTimeAgoLabel);
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange && !this->isActiveWindow()) {
        this->hide();
    }
}

void MainWindow::on_clearAllBtn_clicked()
{
    lastCopiedContent = "";
    db->clearAllClipboardData();
    clearClipboardHistory();
}

void MainWindow::on_fileTypeBox_currentIndexChanged(int index)
{
    typeSelected = static_cast<ContentType>(index);
    loadClipboardItems();
}

void MainWindow::on_searchBar_textChanged(const QString &text)
{
    searchText = text;
    loadClipboardItems();
}
