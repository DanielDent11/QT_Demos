#ifndef ARTIST_H
#define ARTIST_H

#include <QString>
#include <QPixmap>
#include <QJsonObject>
#include <QPixmap>
#include <QJsonArray>

#include "treeitem.h"

class Album;

class Artist : public TreeItem
{
public:
    Artist();

    static Artist* fromJson(const QJsonObject& jsonObj);

    QString getName();
    void setName(const QString& _name);

    QPixmap getPhoto();
    void setPhoto(QPixmap _photo);

    QString getCountry();
    void setCountry(const QString& _country);

    QString getComment();
    void setComment(const QString& _comment);

    QVector<Album* >& getAlbums();
    void setAlbums(QVector<Album* >& _albums);

    QJsonObject toJson();

    TreeItem* getDataItem() const override;

    void setPathToPhoto(QString _pathToPhoto);
    QString getPathToPhoto();
private:
    QString name;
    QPixmap photo;
    QString country;
    QString comment;
    QVector<Album* > albums;
    QString pathToPhoto;
};

#endif // ARTIST_H
