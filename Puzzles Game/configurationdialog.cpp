#include "configurationdialog.h"
#include "ui_configurationdialog.h"

#include <QFileDialog>
#include <QImageReader>
#include <QDebug>

ConfigurationDialog::ConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigurationDialog),
    filePath("")
{
    ui->setupUi(this);
}

ConfigurationDialog::~ConfigurationDialog()
{
    delete ui;
}

QString ConfigurationDialog::imageFilePath() const
{
    return ui->lineEdit->text();
}

QSize ConfigurationDialog::puzzleSize() const
{
    return ui->frame->value();
}

void ConfigurationDialog::on_chooseButton_clicked()
{
    QString _filePath = QFileDialog::getOpenFileName(this, tr("Choose image file"), QString(),
                                                    tr("Images (*.png *.xpm *.jpg)"));
    if (_filePath.isEmpty() || _filePath.isNull())
        return;
    filePath = _filePath;
    QPixmap* pixmap = new QPixmap();
    if (!pixmap->load(filePath))
    {
        QMessageBox::warning(this, tr("Image file is not correct"), tr("The image file you chose is not correct. "
                                                                       "Please try another one."));
        return;
    }
    ui->frame->setPixmapValue(pixmap);
    ui->lineEdit->setText(filePath);
}

void ConfigurationDialog::on_buttonBox_accepted()
{
    if (filePath.isNull() || filePath.isEmpty())
    {
        QMessageBox::warning(this, tr("File not chosen"), tr("Please choose a file and then proceed."));
        return;
    }
    accept();
}

void ConfigurationDialog::on_buttonBox_rejected()
{
    reject();
}
