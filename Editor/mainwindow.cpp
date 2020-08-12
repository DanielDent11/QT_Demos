#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , number_of_the_window(0)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    setWindowTitle("Untitled");
    readSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else
    {
        event->ignore();
    }
}

int MainWindow::number_of_created_files = 1;

void MainWindow::on_actionNew_triggered()
{
    MainWindow *w = new MainWindow(nullptr);
    ++number_of_created_files;
    QString fname = "Untitled" + (number_of_created_files > 1 ? QString::number(number_of_created_files) : "");
    w->setWindowTitle(fname);
    w->number_of_the_window = number_of_created_files;
    int height = this->height();
    int width = this->width();
    w->setGeometry(this->x() + width/2, this->y() + height/2, width, height);
    w->show();
}

void MainWindow::on_actionOpen_triggered()
{
    if (!maybeSave()) {
        return;
    }

    QString file_name =  QFileDialog::getOpenFileName(this, "Open the file");
    if (file_name.isEmpty()) {
        return;
    }
    QFile file(file_name);
    file_path_ = file_name;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "Cannot open the file");
        return;
    }
    setWindowTitle(file_name);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
    setWindowModified(false);
    ui->textEdit->document()->setModified(false);
}

void MainWindow::on_actionSave_triggered()
{
    save();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionSave_as_triggered()
{
    QString file_name =  QFileDialog::getSaveFileName(this, "Save as");
    if (file_name.isEmpty()) {
        return;
    }
    QFile file(file_name);
    file_path_ = file_name;
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "Cannot save the file");
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
    ui->textEdit->document()->setModified(false);
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::closeAllWindows();
}

void MainWindow::on_actionAbout_triggered()
{
    QString about_text;
    about_text = "Author : Danil Popkov\n";
    about_text += "Date: 10/11/2019\n";
    about_text += "(C) MaybeEditor (R)";

    QMessageBox::about(this, "About MaybeEditor", about_text);
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_actionChoose_font_triggered()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected) {
        ui->textEdit->setFont(font);
        writeSettings();
    }
}

void MainWindow::on_textEdit_textChanged()
{
    setWindowModified(true);
    setWindowTitle((file_path_.isEmpty() ? "Untitled" +
        (number_of_the_window > 1 ? QString::number(number_of_the_window) : "") + "[*]" : file_path_ + "[*]"));
}

bool MainWindow::save()
{
    if (file_path_.isEmpty()) {
        QString filename = QFileDialog::getSaveFileName(this, "Save");
        if (filename.isEmpty()) {
            return false;
        }
        file_path_ = filename;
    }

    QFile file(file_path_);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "Cannot save the file");
        return false;
    }
    setWindowTitle(file_path_);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
    ui->textEdit->document()->setModified(false);
    return true;
}

bool MainWindow::maybeSave()
{
    if (!ui->textEdit->document()->isModified()) {
        return true;
    }

    QMessageBox::StandardButton ret =
               QMessageBox::warning(this, tr("Application"),
                                       tr("The document has been modified.\n"
                                          "Do you want to save your changes?"),
                                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                        QMessageBox::Discard);
    switch(ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }

    return true;
}

void MainWindow::writeSettings() {
    QSettings settings("NRNU MEPhI", "MaybeEditor");
    settings.setValue("reader.font", ui->textEdit->font().toString());
}

void MainWindow::readSettings() {
    QSettings settings("NRNU MEPhI", "MaybeEditor");
    QFont font;
    font.fromString(settings.value("reader.font").toString());
    ui->textEdit->setFont(font);
}

