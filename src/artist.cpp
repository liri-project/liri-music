#include "artist.h"
#include "moc_artist.cpp"

#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "song.h"

Artist::Artist() :
    QObject() {
}

Artist::Artist(const QString& artist) :
    QObject(),
    m_artist(artist)
{
}

Artist::Artist(const Artist& other) :
    QObject(),
    m_artist(other.m_artist) {
}

Artist& Artist::operator=(const Artist& other) {
    m_artist = other.m_artist;
    return *this;
}

QList<QObject*> Artist::getSong() const {
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

                songList.append(new Song(path, title, album, artist, art));
            }
        }
    }
    return songList;
}

QString Artist::artist() const {
    return m_artist;
}
