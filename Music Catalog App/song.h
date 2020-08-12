#ifndef SONG_H
#define SONG_H

#include <QString>
#include <QTime>
#include <QJsonObject>
#include <QJsonArray>

#include "treeitem.h"

class Album;

class Song : public TreeItem
{
public:
    Song();
    Song(const QString& _title, const QTime& _duration, int _rating,
         const QString& _comment, QString _fromAlbum);

    static Song* fromJson(const QJsonObject &jsonObj);

    QString getTitle();
    void setTitle(const QString& _title);

    QTime getDuration();
    void setDuration(const QTime& _duration);

    int getRating();
    void setRating(int _rating);

    QString getComment();
    void setComment(const QString& _comment);

    QString getFromAlbum();
    void setFromAlbum(QString _album);

    QJsonObject toJson();

private:
    QString title;
    QTime duration;
    int rating;
    QString comment;
    QString fromAlbum;
};

#endif // SONG_H
