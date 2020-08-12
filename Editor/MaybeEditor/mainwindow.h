#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QFont>
#include <QFontDialog>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_actionUndo_triggered();

    void on_actionSave_as_triggered();

    void on_actionClose_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionChoose_font_triggered();

    void on_textEdit_textChanged();

private:
    Ui::MainWindow *ui;
    QString file_path_;
    static int number_of_created_files;
    int number_of_the_window;

    void readSettings();
    void writeSettings();
    bool save();
    bool maybeSave();
};
#endif // MAINWINDOW_H
