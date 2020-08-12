#include "addartistdialog.h"
#include "ui_addartistdialog.h"

AddArtistDialog::AddArtistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddArtistDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddArtistDialog::verify);
    connect(ui->photoButton, &QAbstractButton::clicked, this, &AddArtistDialog::setPhoto);
}

AddArtistDialog::~AddArtistDialog()
{
    delete ui;
}

void AddArtistDialog::verify()
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

QVariant AddArtistDialog::getName()
{
    return QVariant(ui->nameEdit->text());
}

QVariant AddArtistDialog::getComment()
{
    return QVariant(ui->commentEdit->toPlainText());
}

QVariant AddArtistDialog::getCountry()
{
    return QVariant(ui->countryEdit->text());
}

QPixmap AddArtistDialog::getPhoto()
{
    return QPixmap();
}

QString AddArtistDialog::getPathToPhoto()
{
    return pathToPhoto;
}

void AddArtistDialog::setPhoto()
{

    QString _pathToPhoto = QFileDialog::getOpenFileName(this, "Choose file");

    if (_pathToPhoto.isEmpty())
        return;

    pathToPhoto = _pathToPhoto;

    QPixmap pixmap(pathToPhoto);
    photo = pixmap;
    int w = ui->photoLabel->width();
    int h = ui->photoLabel->height();
    photo = photo.scaled(w, h);
    ui->photoLabel->setPixmap(photo);
}
