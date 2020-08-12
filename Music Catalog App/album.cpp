#include "album.h"
#include "artist.h"
#include "song.h"

Album::Album() :
    TreeItem(QVector<QVariant>()),
    name("[No data]"),
    yearOfRelease(static_cast<quint32>(-1)),
    cover(nullptr),
    genre("[No data]"),
    comment("[No data]"),
    songs(QVector<Song* >()),
    ofArtist("[No data]"),
    pathToCover(":/rec/img/1024px-no_image_available-svg(1).png")
{

}


QString Album::getName()
{
    return name;
}

void Album::setName(const QString& _name)
{
    name = _name;
}

quint32 Album::getYearOfRelease()
{
    return yearOfRelease;
}

void Album::setYearOfRelease(const quint32 _yearOfRelease)
{
    yearOfRelease = _yearOfRelease;
}

QPixmap* Album::getCover()
{
    return cover;
}

void Album::setCover(QPixmap* _cover)
{
    cover = _cover;
}

QString Album::getGenre()
{
    return genre;
}

void Album::setGenre(const QString& _genre)
{
    genre = _genre;
}

QString Album::getComment()
{
    return comment;
}

void Album::setComment(const QString& _comment)
{
    comment = _comment;
}

QVector<Song* >& Album::getSongs()
{
    return songs;
}

void Album::setSongs(QVector<Song *>& _songs)
{
    songs = _songs;
}

QString Album::getArtist()
{
    return ofArtist;
}

void Album::setArtist(QString _artist)
{
    ofArtist = _artist;
}

Album* Album::fromJson(const QJsonObject &jsonObj)
{
    Album* album = new Album();

    album->setName(jsonObj.value("name").toString());

    QString pathToCover = jsonObj.value("pathToCover").toString();
    QPixmap *cover = new QPixmap(pathToCover);
    album->setCover(cover);
    album->pathToCover = pathToCover;
    album->setGenre(jsonObj.value("genre").toString());
    album->setComment(jsonObj.value("comment").toString());
    album->setYearOfRelease(jsonObj.value("yearOfRelease").toString().toInt());
    album->setArtist(jsonObj.value("artist").toString());

    QJsonArray jsonSongs = jsonObj.value("songs").toArray();

    QVector<Song*> songs;
    foreach (QJsonValue songVal, jsonSongs) {
       Song* song = Song::fromJson(songVal.toObject());
       songs.append(song);
    }

    album->setSongs(songs);

    return album;
}

QJsonObject Album::toJson()
{
    QJsonObject jsonObj;
    jsonObj["name"] = name;
    jsonObj["yearOfRelease"] = QString::number(yearOfRelease);
    jsonObj["pathToCover"] = pathToCover;
    jsonObj["comment"] = comment;
    jsonObj["artist"] = this->getArtist();
    jsonObj["genre"] = genre;
    QJsonArray jsonSongs;
    foreach(Song* song, songs)
    {
        jsonSongs.append(song->toJson());
    }
    jsonObj["songs"] = jsonSongs;
    return jsonObj;
}

