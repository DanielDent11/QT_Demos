#include "addalbumdialog.h"
#include "ui_addalbumdialog.h"

AddAlbumDialog::AddAlbumDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAlbumDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddAlbumDialog::verify);
    connect(ui->coverButton, &QAbstractButton::clicked, this, &AddAlbumDialog::setCover);
}

AddAlbumDialog::~AddAlbumDialog()
{
    delete ui;
}

QString AddAlbumDialog::getName()
{
    return ui->nameEdit->text();
}

QString AddAlbumDialog::getComment()
{
    return ui->commentEdit->toPlainText();
}

QString AddAlbumDialog::getGenre()
{
    return ui->genreEdit->text();
}

QString AddAlbumDialog::getPathToCover()
{
    return pathToCover;
}

int AddAlbumDialog::getYearOfRelease()
{
    return ui->yearEdit->text().toInt();
}

QString AddAlbumDialog::getArtist()
{
    return ui->artistEdit->text();
}

void AddAlbumDialog::verify()
{
    if (!ui->nameEdit->text().isEmpty()) {
        accept();
    } else {
        QMessageBox::StandardButton answer;
        answer = QMessageBox::warning(this, tr("Incomplete Form"),
            tr("The form does not contain all the necessary information.\n"
               "Do you want to discard it?"),
            QMessageBox::Yes | QMessageBox::No);

        if (answer == QMessageBox::Yes)
            reject();
    }
}

void AddAlbumDialog::setCover()
{
    QString _pathToCover = QFileDialog::getOpenFileName();

    if (_pathToCover.isEmpty())
        return;

    pathToCover = _pathToCover;

    QPixmap _cover(pathToCover);
    _cover = _cover.scaled(ui->coverLabel->width(), ui->coverLabel->height());
    ui->coverLabel->setPixmap(_cover);
}
