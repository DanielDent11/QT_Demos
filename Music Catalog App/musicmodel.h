#ifndef MUSICMODEL_H
#define MUSICMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "treeitem.h"
#include "artist.h"
#include "album.h"
#include "song.h"

class MusicModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    MusicModel(const QStringList& headers, const QJsonArray& data,
               QObject *parent = nullptr);
    ~MusicModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    TreeItem* getRootItem();
    void setFlags(Qt::ItemFlag _enabledFlags);

    TreeItem *getItem(const QModelIndex &index) const;

private:
    void setupModelData(const QJsonArray &data, TreeItem *parent);

    TreeItem *rootItem;
};

#endif // MUSICMODEL_H
