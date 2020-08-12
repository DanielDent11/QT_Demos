#ifndef ALBUM_H
#define ALBUM_H

#include <QPixmap>
#include <QJsonArray>
#include <QJsonObject>
#include <QPixmap>

#include "treeitem.h"

class Song;
class Artist;

class Album : public TreeItem
{
public:
    Album();

    static Album* fromJson(const QJsonObject& jsonObj);

    QString getName();
    void setName(const QString& _name);

    quint32 getYearOfRelease();
    void setYearOfRelease(const quint32 _yearOfRelease);

    QPixmap* getCover();
    void setCover(QPixmap* _cover);

    QString getGenre();
    void setGenre(const QString& _genre);

    QString getComment();
    void setComment(const QString& _comment);

    QVector<Song* >& getSongs();
    void setSongs(QVector<Song* >& _songs);

    QString getArtist();
    void setArtist(QString _artist);

    QJsonObject toJson();
    QString pathToCover;
private:
    QString name;
    quint32 yearOfRelease;
    QPixmap* cover;
    QString genre;
    QString comment;
    QVector<Song* > songs;
    QString ofArtist;


};

#endif // ALBUM_H
