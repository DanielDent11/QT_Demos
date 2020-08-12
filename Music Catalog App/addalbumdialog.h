#ifndef ADDALBUMDIALOG_H
#define ADDALBUMDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>

namespace Ui {
class AddAlbumDialog;
}

class AddAlbumDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAlbumDialog(QWidget *parent = nullptr);
    ~AddAlbumDialog();

    QString getName();
    QString getComment();
    QString getGenre();
    QString getPathToCover();
    int getYearOfRelease();
    QString getArtist();

    QString pathToCover;

    Ui::AddAlbumDialog *ui;
public slots:
    void verify();
    void setCover();

private:

};

#endif // ADDALBUMDIALOG_H
