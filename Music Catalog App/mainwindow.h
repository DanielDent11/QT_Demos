#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QFile>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>
#include <QCloseEvent>

#include <musicmodel.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool maybeSave();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onActionAddArtist();

    void on_actionAdd_Album_triggered();

    void on_actionAdd_Song_triggered();

    void updateActions();

    void on_actionRemove_Artist_triggered();

    void on_actionRemove_Album_triggered();

    void on_actionRemove_Song_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void onDataChanged();

    void on_actionSave_as_triggered();

    void onCustomContextMenu(const QPoint &point);

    void onEditArtist();

    void onEditAlbum();

    void on_actionClose_triggered();

private:
    Ui::MainWindow *ui;
    MusicModel *model;
    QString fileName;
    QString filePath;

    bool modified;
};
#endif // MAINWINDOW_H
