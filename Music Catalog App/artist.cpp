#include "artist.h"
#include "album.h"

Artist::Artist() :
    TreeItem(QVector<QVariant>()),
    name("[No data]"),
    photo(QPixmap()),
    country("[No data]"),
    comment("[No data]"),
    albums(QVector<Album* >()),
    pathToPhoto(":/rec/img/1024px-no_image_available-svg(1).png")
{

}

QString Artist::getName()
{
    return name;
}

void Artist::setName(const QString& _name)
{
    name = _name;
}

QPixmap Artist::getPhoto()
{
    return photo;
}

void Artist::setPhoto(QPixmap _photo)
{
    photo = _photo;
}

QString Artist::getCountry()
{
    return country;
}

void Artist::setCountry(const QString& _country)
{
    country = _country;
}

QString Artist::getComment()
{
    return comment;
}

void Artist::setComment(const QString& _comment)
{
    comment = _comment;
}

QVector<Album*>& Artist::getAlbums()
{
    return albums;
}

void Artist::setAlbums(QVector<Album *>& _albums)
{
    albums = _albums;
}

Artist* Artist::fromJson(const QJsonObject &jsonObj)
{
    Artist *artist = new Artist();

    artist->setName(jsonObj.value("name").toString());

    QString pathToPhoto = jsonObj.value("pathToPhoto").toString();
    QPixmap photo = QPixmap(pathToPhoto);
    artist->setPhoto(photo);
    artist->setPathToPhoto(pathToPhoto);

    artist->setComment(jsonObj.value("comment").toString());
    artist->setCountry(jsonObj.value("country").toString());

    QJsonArray jsonAlbums = jsonObj.value("albums").toArray();

    QVector<Album*> albums;

    foreach (QJsonValue albumVal, jsonAlbums) {
       Album* album = Album::fromJson(albumVal.toObject());
       albums.append(album);
    }

    artist->setAlbums(albums);

    return artist;
}

QJsonObject Artist::toJson()
{
    QJsonObject jsonObj;
    jsonObj["name"] = name;
    jsonObj["country"] = country;
    jsonObj["comment"] = comment;
    jsonObj["pathToPhoto"] = pathToPhoto;
    QJsonArray jsonAlbums;
    foreach(Album* album, albums)
    {
        jsonAlbums.append(album->toJson());
    }
    jsonObj["albums"] = jsonAlbums;
    return jsonObj;
}

TreeItem* Artist::getDataItem() const
{
    return nullptr;
}

void Artist::setPathToPhoto(QString _pathToPhoto)
{
    pathToPhoto = _pathToPhoto;
}

QString Artist::getPathToPhoto()
{
    return pathToPhoto;
}
