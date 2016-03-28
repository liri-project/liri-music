#include "albumobject.h"
#include "moc_albumobject.cpp"

AlbumObject::AlbumObject(const QString& title, const QString& artist, const QString& art) :
    QObject(), m_title(title), m_artist(artist), m_art(art) {
}

QString AlbumObject::title() {
    return m_title;
}

QString AlbumObject::artist() {
    return m_artist;
}

QString AlbumObject::art() {
    return m_art;
}

QList<QObject*> AlbumObject::getSong() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("vinylusic");
    QList<QObject*> songList;

    if(db.open()) {
        QSqlQuery getSongs;
        getSongs.prepare("Select * FROM Songs where album='"+ m_title +"'");
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
