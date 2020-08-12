#ifndef TREEITEM_H
#define TREEITEM_H

#include<QVector>
#include<QVariant>

enum ItemType {
    ROOT,
    ARTIST,
    ALBUM,
    SONG
};

class Artist;
class Song;
class Album;

class TreeItem
{
public:
    TreeItem(const QVector<QVariant> &_itemData,
             TreeItem *parent = nullptr);
    ~TreeItem();

    TreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    TreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);
    virtual TreeItem* getDataItem() const;
    ItemType getItemType();
    void setItemType(ItemType type);

    Song* song;
    Album* album;
    Artist* artist;
private:
    QVector<QVariant> itemData;
    QList<TreeItem*> childItems;
    TreeItem *parentItem;

    ItemType itemType;
};

#endif // TREEITEM_H
