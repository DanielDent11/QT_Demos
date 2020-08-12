#include "musicmodel.h"
#include "artist.h"
#include "album.h"
#include "song.h"

MusicModel::MusicModel(const QStringList &headers, const QJsonArray &data,
                       QObject *parent) :
    QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach(QString header, headers) {
        rootData << header;
    }
    rootItem = new TreeItem(rootData);
    rootItem->setItemType(ROOT);
    setupModelData(data, rootItem);
}

MusicModel::~MusicModel()
{
    delete rootItem;
}

int MusicModel::columnCount(const QModelIndex &) const
{
    return rootItem->columnCount();
}

QVariant MusicModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags MusicModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    TreeItem* item = getItem(index);

    ItemType type = item->getItemType();

    if (type == ROOT)
    {
        if (index.column() >= 0 && index.column() < 4)
        {
            return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
        } else {
            //return QAbstractItemModel::flags(index);
            return ~(Qt::ItemIsSelectable|Qt::ItemIsEnabled) | QAbstractItemModel::flags(index);
        }
    } else if (type == ARTIST)
    {
        if (index.column() == 0)
        {
            return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
        } else {
            //return QAbstractItemModel::flags(index);
            return QAbstractItemModel::flags(index);
        }
    } else if (type == ALBUM)
    {
        if (index.column() == 0)
        {
            return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
        } else {
            //return QAbstractItemModel::flags(index);
            return ~(Qt::ItemIsSelectable|Qt::ItemIsEnabled) | QAbstractItemModel::flags(index);
        }
    } else if (type == SONG)
    {
        if (index.column() >= 0 && index.column() < 4)
        {
            return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
        } else {
           //return QAbstractItemModel::flags(index);
           return ~(Qt::ItemIsSelectable|Qt::ItemIsEnabled) | QAbstractItemModel::flags(index);
        }
    }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

TreeItem *MusicModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant MusicModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex MusicModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool MusicModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool MusicModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    if (success)
        emit dataChanged(QModelIndex(), QModelIndex(), {Qt::EditRole});

    return success;
}

QModelIndex MusicModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool MusicModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool MusicModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    if (success)
        emit dataChanged(QModelIndex(), QModelIndex(), {Qt::EditRole});

    return success;
}

int MusicModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool MusicModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index, {role});

    return result;
}

bool MusicModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void MusicModel::setupModelData(const QJsonArray& data, TreeItem *parent)
{
    foreach (QJsonValue artistVal, data)
    {
        Artist* artist = Artist::fromJson(artistVal.toObject());


        parent->insertChildren(parent->childCount(), 1, 1);
        parent->child(parent->childCount() - 1)->setData(0, artist->getName());

        TreeItem *artistItem = parent->child(parent->childCount() - 1);
        artistItem->setItemType(ARTIST);
        artistItem->artist = artist;
        artistItem->setData(0, artist->getName());

        foreach(Album* album, artist->getAlbums())
        {
            artistItem->insertChildren(artistItem->childCount(), 1, 1);

            TreeItem *albumItem = artistItem->child(artistItem->childCount() - 1);
            albumItem->album = album;
            albumItem->setItemType(ALBUM);
            albumItem->setData(0, album->getName());

            foreach(Song* song, album->getSongs())
            {
                albumItem->insertChildren(albumItem->childCount(), 1, 4);

                TreeItem *songItem = albumItem->child(albumItem->childCount() - 1);
                songItem->setItemType(SONG);
                songItem->song = song;

                songItem->setData(0, song->getTitle());
                songItem->setData(1, song->getDuration().toString());
                songItem->setData(2, song->getRating());
                songItem->setData(3, song->getComment());
            }
        }
    }
}

TreeItem* MusicModel::getRootItem()
{
    return rootItem;
}
