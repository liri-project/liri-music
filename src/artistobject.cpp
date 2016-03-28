#include "artistobject.h"
#include "moc_artistobject.cpp"

#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "songobject.h"

ArtistObject::ArtistObject() :
    QObject() {
}

ArtistObject::ArtistObject(const QString& artist) :
    QObject(),
    m_artist(artist)
{
}

ArtistObject::ArtistObject(const ArtistObject& other) :
    QObject(),
    m_artist(other.m_artist) {
}

ArtistObject& ArtistObject::operator=(const ArtistObject& other) {
    m_artist = other.m_artist;
    return *this;
}

QList<QObject*> ArtistObject::getSong() const {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhot");
    db.setDatabaseName("vinylmusic");
    QList<QObject*> songList;

    if(db.open()) {
        QSqlQuery getSongs;
        getSongs.prepare("Select * FROM Songs where artist='"+ m_artist +"'");
        if(getSongs.exec()) {
            while(getSongs.next()) {
                QString title = getSongs.value(2).toString();
                QString path = getSongs.value(1).toString();
                QString album = getSongs.value(4).toString();
                QString artist = getSongs.value(3).toString();
                QString art = getSongs.value(5).toString();

                songList.append(new SongObject(path, title, album, artist, art));
            }
        }
    }
    return songList;
}

QString ArtistObject::artist() const {
    return m_artist;
}
