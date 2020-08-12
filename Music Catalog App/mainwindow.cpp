#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addartistdialog.h"
#include "ui_addartistdialog.h"
#include "addalbumdialog.h"
#include "ui_addalbumdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      fileName("Undefined"),
      filePath(""),
      modified(false)
{
    ui->setupUi(this);
    this->setWindowTitle("MaybeMusic - Welcome");

    QStringList headers;
    headers << "Track" << "Duration" << "Rating" << "Notes";

    QJsonArray jsonData;
    model = new MusicModel(headers, jsonData);

    ui->view->setModel(model);
    ui->view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->actionSave->setEnabled(false);

    connect(ui->actionAdd_Artist, &QAction::triggered, this, &MainWindow::onActionAddArtist);
    connect(ui->menuActions_2, &QMenu::aboutToShow, this, &MainWindow::updateActions);
    connect(ui->view->model(), &QAbstractItemModel::dataChanged, this, &MainWindow::onDataChanged);
    connect(ui->view->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::updateActions);
    ui->view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->view, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    updateActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {

        event->accept();
    } else
    {
        event->ignore();
    }
}

void MainWindow::onActionAddArtist()
{

    AddArtistDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        QModelIndex index = ui->view->selectionModel()->currentIndex();

        if (!model->insertRow(index.row()+1, index.parent()))
            return;

        updateActions();

        QModelIndex child = model->index(index.row()+1, 0, index.parent());
        model->setData(child, dialog.getName(), Qt::EditRole);

        Artist* artist = new Artist();
        artist->setName(dialog.getName().toString());
        artist->setCountry(dialog.getCountry().toString());
        artist->setPhoto(dialog.getPhoto());
        artist->setPathToPhoto(dialog.getPathToPhoto());
        artist->setComment(dialog.getComment().toString());

        model->getItem(child)->artist = artist;
        model->getItem(child)->setItemType(ARTIST);

        updateActions();
    }
}

void MainWindow::on_actionAdd_Album_triggered()
{
    AddAlbumDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        QModelIndex index = ui->view->selectionModel()->currentIndex();

        if (model->getItem(index)->getItemType() == ARTIST) {

            if (!model->insertRow(0, index))
                return;

            QModelIndex child = model->index(0, 0, index);
            model->setData(child, QVariant(dialog.getName()), Qt::EditRole);
            Album* album = new Album();
            album->setName(dialog.getName());
            album->setGenre(dialog.getGenre());
            album->setArtist(dialog.getArtist());
            album->pathToCover = dialog.getPathToCover();
            album->setComment(dialog.getComment());
            album->setYearOfRelease(dialog.getYearOfRelease());

            model->getItem(index)->artist->getAlbums().insert(0, album);

            model->getItem(child)->album = album;
            model->getItem(child)->setItemType(ALBUM);


            ui->view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                                    QItemSelectionModel::ClearAndSelect);
            updateActions();
        } else {
            QModelIndex index = ui->view->selectionModel()->currentIndex();

            if (!model->insertRow(index.row()+1, index.parent()))
                return;

            updateActions();

            QModelIndex child = model->index(index.row()+1, 0, index.parent());
            model->setData(child, dialog.getName(), Qt::EditRole);

            Album* album = new Album();
            album->setName(dialog.getName());
            album->setGenre(dialog.getGenre());
            album->setArtist(dialog.getArtist());
            album->pathToCover = dialog.getPathToCover();
            album->setComment(dialog.getComment());
            album->setYearOfRelease(dialog.getYearOfRelease());

            QModelIndex parent = index.parent();
            model->getItem(parent)->artist->getAlbums().insert(index.row()+1, album);

            model->getItem(child)->album = album;
            model->getItem(child)->setItemType(ALBUM);
        }
    }
}

void MainWindow::on_actionAdd_Song_triggered()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();

    if (model->getItem(index)->getItemType() == ALBUM)
    {
        if (!model->insertRow(0, index))
            return;

        Song* song = new Song();
        song->setTitle("[No data]");
        song->setDuration(QTime::fromString("00:00:00"));
        song->setRating(0);
        song->setComment("[No data]");
        song->setFromAlbum(model->getItem(index)->album->getName());

        for (int column = 0; column < model->columnCount(index); ++column) {
            QModelIndex child = model->index(0, column, index);
            model->getItem(child)->song = song;
            model->getItem(child)->setItemType(SONG);

            if (column == 1) {
                model->setData(child, QVariant("00:00:00"), Qt::EditRole);
            } else if (column == 2)
            {
                model->setData(child, QVariant(0), Qt::EditRole);
            } else {
                model->setData(child, QVariant("[No data]"), Qt::EditRole);
            }

            if (!model->headerData(column, Qt::Horizontal).isValid())
                model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);
        }

        model->getItem(index)->album->getSongs().insert(0, song);

        ui->view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                                QItemSelectionModel::ClearAndSelect);

        updateActions();
    } else {

        QModelIndex index = ui->view->selectionModel()->currentIndex();

        if (!model->insertRow(index.row()+1, index.parent()))
            return;

        updateActions();

        Song* song = new Song();
        song->setTitle("[No data]");
        song->setDuration(QTime::fromString("00:00:00"));
        song->setRating(0);
        song->setComment("[No data]");
        song->setFromAlbum(model->getItem(index.parent())->album->getName());

        for (int column = 0; column < model->columnCount(index.parent()); ++column) {
            QModelIndex child = model->index(index.row()+1, column, index.parent());
            model->getItem(child)->song = song;
            model->getItem(child)->setItemType(SONG);
            if (column == 1) {
                model->setData(child, QVariant("00:00:00"), Qt::EditRole);
            } else if (column == 2)
            {
                model->setData(child, QVariant(0), Qt::EditRole);
            } else {
                model->setData(child, QVariant("[No data]"), Qt::EditRole);
            }
        }

        model->getItem(index.parent())->album->getSongs().insert(index.row()+1, song);
    }
}

void MainWindow::updateActions()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();
    ItemType type = model->getItem(index)->getItemType();
    qDebug(QString::number(type).toStdString().c_str());

    if (type == ARTIST)
    {
        ui->actionRemove_Artist->setEnabled(true);
        ui->actionRemove_Song->setEnabled(false);
        ui->actionRemove_Album->setEnabled(false);

        ui->actionAdd_Song->setEnabled(false);
        ui->actionAdd_Artist->setEnabled(true);
        ui->actionAdd_Album->setEnabled(true);
    } else if (type == SONG)
    {
        ui->actionRemove_Song->setEnabled(true);
        ui->actionRemove_Album->setEnabled(false);
        ui->actionRemove_Artist->setEnabled(false);

        ui->actionAdd_Song->setEnabled(true);
        ui->actionAdd_Album->setEnabled(false);
        ui->actionAdd_Artist->setEnabled(false);

    } else if (type == ALBUM)
    {
        ui->actionRemove_Album->setEnabled(true);
        ui->actionRemove_Song->setEnabled(false);
        ui->actionRemove_Artist->setEnabled(false);

        ui->actionAdd_Artist->setEnabled(false);
        ui->actionAdd_Song->setEnabled(true);
        ui->actionAdd_Album->setEnabled(true);

    } else if (type == ROOT)
    {
        ui->actionRemove_Artist->setEnabled(false);
        ui->actionRemove_Song->setEnabled(false);
        ui->actionRemove_Album->setEnabled(false);

        ui->actionAdd_Song->setEnabled(false);
        ui->actionAdd_Artist->setEnabled(true);
        ui->actionAdd_Album->setEnabled(false);
    }
}

void MainWindow::on_actionRemove_Artist_triggered()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}

void MainWindow::on_actionRemove_Album_triggered()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}

void MainWindow::on_actionRemove_Song_triggered()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}

void MainWindow::on_actionNew_triggered()
{
    QStringList headers;
    headers << "Track" << "Duration" << "Rating" << "Notes";

    modified = false;
    filePath = "";
    fileName = "Undefined";
    this->setWindowTitle(fileName);

    QJsonArray jsonData;

    model = new MusicModel(headers, jsonData);
    QModelIndex index = model->index(0, 0);

    ui->view->setModel(model);
    ui->view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->view->model(), &QAbstractItemModel::dataChanged, this, &MainWindow::onDataChanged);
}

void MainWindow::on_actionOpen_triggered()
{
    filePath = QFileDialog::getOpenFileName(this, "Choose file");

    if (filePath.isEmpty())
        return;

    modified = false;

    QString fp = '/'+filePath;
    fileName = fp.mid(fp.lastIndexOf('/')+1);
    this->setWindowTitle(fileName);

    QStringList headers;
    headers << "Track" << "Duration" << "Rating" << "Notes";

    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QByteArray data;
    QDataStream in(&file);
    in >> data;
    //data.append(file.readAll());
    QJsonDocument jsonDoc = QJsonDocument::fromBinaryData(data);
    //QJsonParseError parseError;
    //QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);
    QJsonArray jsonData = jsonDoc.array();

    model = new MusicModel(headers, jsonData);
    QModelIndex index = model->index(0, 0);

    ui->view->setModel(model);
    ui->view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->view->model(), &QAbstractItemModel::dataChanged, this, &MainWindow::onDataChanged);
}

void MainWindow::on_actionSave_triggered()
{
    if (filePath.isEmpty()) {
        on_actionSave_as_triggered();
        return;
    }

    QJsonArray jsonArray;

    for (int row = 0; row < model->rowCount(); ++row)
    {
        QModelIndex index = model->index(row, 0, QModelIndex());
        TreeItem* item = model->getItem(index);
        jsonArray.append(item->artist->toJson());
    }

    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << QJsonDocument(jsonArray).toBinaryData();
    //file.write(QJsonDocument(jsonArray).toJson());
    file.close();

    modified = false;
    this->setWindowTitle(fileName);
    ui->actionSave->setEnabled(false);
}

void MainWindow::onDataChanged()
{
    if (modified)
        return;
    modified = true;
    this->setWindowTitle("*"+fileName);
    ui->actionSave->setEnabled(true);
    updateActions();
}

void MainWindow::on_actionSave_as_triggered()
{
    QString _filePath = QFileDialog::getSaveFileName(this, "Save as");

    if (_filePath.isEmpty())
        return;

    filePath = _filePath;

    QString fp = '/'+filePath;
    fileName = fp.mid(fp.lastIndexOf('/')+1);

    QJsonArray jsonArray;

    for (int row = 0; row < model->rowCount(); ++row)
    {
        QModelIndex index = model->index(row, 0, QModelIndex());
        TreeItem* item = model->getItem(index);
        jsonArray.append(item->artist->toJson());
    }

    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << QJsonDocument(jsonArray).toBinaryData();
    file.close();

    modified = false;
    this->setWindowTitle(fileName);
    ui->actionSave->setEnabled(false);
}

bool MainWindow::maybeSave()
{
    if (!modified) {
        return true;
    }

    QMessageBox::StandardButton ret =
               QMessageBox::warning(this, tr("Application"),
                                       tr("The document has been modified.\n"
                                          "Do you want to save your changes?"),
                                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                        QMessageBox::Discard);
    switch(ret) {
    case QMessageBox::Save: {
        on_actionSave_triggered();
        return true;
    }
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }

    return true;
}

void MainWindow::onCustomContextMenu(const QPoint &point)
{
    QModelIndex index = ui->view->indexAt(point);
    if (model->getItem(index)->getItemType() == SONG)
    {
        return;
    }

    QMenu *menu=new QMenu(this);
    QAction* editAction = new QAction("Properties", this);
    menu->addAction(editAction);
    if (model->getItem(index)->getItemType() == ARTIST)
    {
        connect(editAction, SIGNAL(triggered()), this, SLOT(onEditArtist()));
    } else {
        connect(editAction, SIGNAL(triggered()), this, SLOT(onEditAlbum()));
    }

    menu->popup(ui->view->viewport()->mapToGlobal(point));
}

void MainWindow::onEditArtist()
{
    AddArtistDialog dialog(this);
    QModelIndex index = ui->view->selectionModel()->currentIndex();
    Artist* artist = model->getItem(index)->artist;

    dialog.setWindowTitle("Artist - [Editin/Preview]");
    dialog.ui->nameEdit->setText(artist->getName());
    dialog.ui->countryEdit->setText(artist->getCountry());
    dialog.ui->commentEdit->setText(artist->getComment());
    dialog.pathToPhoto = artist->getPathToPhoto();

    if (artist->getPathToPhoto().isEmpty() || artist->getPathToPhoto().isNull())
    {
        QPixmap photo(":/rec/img/1024px-no_image_available-svg(1).png");
        dialog.ui->photoLabel->setPixmap(photo);
    } else {
        QPixmap photo(artist->getPathToPhoto());
        dialog.ui->photoLabel->setPixmap(photo);
    }

    if (dialog.exec() == QDialog::Accepted)
    {
        updateActions();

        model->setData(index, dialog.getName(), Qt::EditRole);

        Artist* artist = model->getItem(index)->artist;
        artist->setName(dialog.getName().toString());
        artist->setCountry(dialog.getCountry().toString());
        artist->setPhoto(dialog.getPhoto());
        artist->setPathToPhoto(dialog.getPathToPhoto());
        artist->setComment(dialog.getComment().toString());

        updateActions();
    }
}

void MainWindow::onEditAlbum()
{
    AddAlbumDialog dialog(this);
    QModelIndex index = ui->view->selectionModel()->currentIndex();
    Album* album = model->getItem(index)->album;
    dialog.pathToCover = album->pathToCover;

    dialog.setWindowTitle("Album - [Editing/Preview]");
    dialog.ui->nameEdit->setText(album->getName());
    dialog.ui->genreEdit->setText(album->getGenre());
    dialog.ui->yearEdit->setText(QString::number(album->getYearOfRelease()));
    dialog.ui->commentEdit->setText(album->getComment());
    dialog.ui->artistEdit->setText(album->getArtist());

    if (!album->pathToCover.isNull() && !album->pathToCover.isEmpty()){
        QPixmap cover(album->pathToCover);
        dialog.ui->coverLabel->setPixmap(cover);
    } else {
        QPixmap cover(":/rec/img/1024px-no_image_available-svg(1).png");
        dialog.ui->coverLabel->setPixmap(cover);
    }

    if (dialog.exec() == QDialog::Accepted)
    {
        model->setData(index, QVariant(dialog.getName()), Qt::EditRole);
        album->setName(dialog.getName());
        album->setGenre(dialog.getGenre());
        album->setArtist(dialog.getArtist());
        album->pathToCover = dialog.getPathToCover();
        album->setComment(dialog.getComment());
        album->setYearOfRelease(dialog.getYearOfRelease());

        ui->view->selectionModel()->setCurrentIndex(index,
                                                    QItemSelectionModel::ClearAndSelect);
        updateActions();
    }
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}
