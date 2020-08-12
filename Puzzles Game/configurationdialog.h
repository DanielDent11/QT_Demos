#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class ConfigurationDialog;
}

class ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog(QWidget *parent = nullptr);
    ~ConfigurationDialog();

    QString imageFilePath() const;
    QSize puzzleSize() const;

private slots:
    void on_chooseButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ConfigurationDialog *ui;
    QString filePath;
};

#endif // CONFIGURATIONDIALOG_H
