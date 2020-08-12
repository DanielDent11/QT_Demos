#ifndef ADDARTISTDIALOG_H
#define ADDARTISTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QVariant>
#include <QAction>
#include <QFileDialog>

namespace Ui {
class AddArtistDialog;
}

class AddArtistDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddArtistDialog(QWidget *parent = nullptr);
    ~AddArtistDialog();

    QVariant getName();
    QVariant getCountry();
    QString getPathToPhoto();
    QPixmap getPhoto();
    QVariant getComment();

    QString pathToPhoto;

    Ui::AddArtistDialog *ui;
public slots:
    void verify();
    void setPhoto();

private:

    QPixmap photo;
};

#endif // ADDARTISTDIALOG_H
