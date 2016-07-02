#include "song.h"
#include "moc_song.cpp"

Song::Song() :
    QObject() {

}


Song::Song(quint64 id, const QString& path, const QString& title, quint64 album,
                       quint64 artist, const QString& art) :
    QObject(),
    m_path(path),
    m_title(title),
    m_album(album),
    m_artist(artist),
    m_art(art),
    m_id(id)
{
}

Song::Song(const Song& other) :
    QObject(),
    m_path(other.m_path),
    m_title(other.m_title),
    m_album(other.m_album),
    m_artist(other.m_artist),
    m_art(other.m_art),
    m_id(other.m_id) {
}

Song& Song::operator=(const Song& other) {
    m_path = other.m_path;
    m_title = other.m_title;
    m_album = other.m_album;
    m_artist = other.m_artist;
    m_art = other.m_art;
    m_id = other.m_id;
    return *this;
}

QString Song::path() const {
    return m_path;
}

QString Song::title() const {
    return m_title;
}



quint64 Song::album() const {
    return m_album;
}

quint64 Song::artist() const {
    return m_artist;
}

QString Song::art() const {
    return m_art;
}

quint64 Song::id() const {
    return m_id;
}

void Song::setPath(const QString& path) {
    m_path = path;
}

void Song::setTitle(const QString& title) {
    m_title = title;
}

void Song::setAlbum(quint64 album) {
    m_album = album;
}

void Song::setArtist(quint64 artist) {
    m_artist = artist;
}

void Song::setArt(const QString& art) {
    m_art = art;
}

void Song::setId(quint64 id) {
    m_id = id;
}

