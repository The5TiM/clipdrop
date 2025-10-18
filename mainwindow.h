#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainterPath>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <QClipboard>
#include <QMimeData>
#include <QImage>
#include <QUrl>
#include <QStandardPaths>


#include <qhotkey.h>
#include <clipboarditem.h>
#include <clipboarddata.h>
#include <clipboarddatabase.h>
#include <contentanalyzer.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_clearAllBtn_clicked();
    void on_fileTypeBox_currentIndexChanged(int index);
    void on_searchBar_textChanged(const QString &text);

private:
    Ui::MainWindow *ui;

    void initialize();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint currentPosition;
    QPoint newPosition;

    void changeEvent(QEvent *event);

    void createShowAndHideWindowShortcut();
    QHotkey *toggleShortcut;

    void makeRoundedCorners(int r);
    void moveWindowToCursorPosition();

    void startClipboardMonitoring();
    QClipboard *clipboard;
    QString lastCopiedContent;

    void clearLayout(QVBoxLayout *layout, QLabel *label);
    void clearClipboardHistory();

    void addNewClipboardItem(const QMimeData *mimeData);
    void addNewClipboardItem(const ClipboardData &data, QVBoxLayout *layout);
    void handleItemRemoval(ClipboardItem *item, QVBoxLayout *layout);
    QLabel* getLabelInLayout(QVBoxLayout *layout);

    ClipboardDatabase *db;
    void initializeDatabase();
    void loadClipboardItems();

    QString searchText = "";
    ContentType typeSelected = ContentType::all;
};
#endif // MAINWINDOW_H
