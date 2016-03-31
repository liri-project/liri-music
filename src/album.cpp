#include "album.h"
#include "moc_album.cpp"

Album::Album() :
    QObject() {
}

Album::Album(const QString& title, const QString& artist, const QString& art) :
    QObject(), m_title(title), m_artist(artist), m_art(art) {
}

Album::Album(const Album& other) :
    QObject(), m_title(other.m_title), m_artist(other.m_artist), m_art(other.m_art) {
}

Album& Album::operator=(const Album& other) {
    m_title = other.m_title;
    m_artist = other.m_artist;
    m_art = other.m_art;
    return *this;
}

QString Album::title() const {
    return m_title;
}

QString Album::artist() const {
    return m_artist;
}

QString Album::art() const {
    return m_art;
}

QList<QObject*> Album::getSong() {
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
