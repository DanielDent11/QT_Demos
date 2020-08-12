#include "song.h"

Song::Song() :
    TreeItem(QVector<QVariant>()),
    title("[No data]"),
    duration(QTime(0, 0, 0, 0)),
    rating(0),
    comment("[No data]"),
    fromAlbum("[No data]")
{

}

Song::Song(const QString& _title, const QTime& _duration, int _rating,
           const QString& _comment, QString _fromAlbum) :
    TreeItem(QVector<QVariant>())
{
    title = _title;
    duration = _duration;
    rating = _rating;
    comment = _comment;
    fromAlbum = _fromAlbum;
}

QString Song::getTitle()
{
    return title;
}

void Song::setTitle(const QString& _title)
{
    title = _title;
}

QTime Song::getDuration()
{
    return duration;
}

void Song::setDuration(const QTime &_duration)
{
    duration = _duration;
}

int Song::getRating()
{
    return rating;
}

void Song::setRating(int _rating)
{
    rating = _rating;
}

QString Song::getComment() {
    return comment;
}

void Song::setComment(const QString &_comment)
{
    comment = _comment;
}

QString Song::getFromAlbum()
{
    return fromAlbum;
}

void Song::setFromAlbum(QString _fromAlbum)
{
    fromAlbum = _fromAlbum;
}

Song* Song::fromJson(const QJsonObject &jsonObj)
{
    Song* song = new Song();

    song->setTitle(jsonObj.value("title").toString());
    song->setRating(jsonObj.value("rating").toString().toInt());
    song->setComment(jsonObj.value("comment").toString());
    song->setFromAlbum(jsonObj.value("fromAlbum").toString());

    QTime duration = QTime::fromString(jsonObj.value("duration").toString());
    song->setDuration(duration);

    return song;
}

QJsonObject Song::toJson()
{
    QJsonObject jsonObj;
    jsonObj["title"] = title;
    jsonObj["duration"] = duration.toString();
    jsonObj["comment"] = comment;
    jsonObj["fromAlbum"] = fromAlbum;
    jsonObj["rating"] = QString::number(rating);
    return jsonObj;
}
