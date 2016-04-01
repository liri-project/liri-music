#include "song.h"
#include "moc_song.cpp"

Song::Song() :
    QObject() {
}

Song::Song(const QString& path, const QString& title, const QString& album,
                       const QString& artist, const QString& art) :
    QObject(),
    m_path(path),
    m_title(title),
    m_album(album),
    m_artist(artist),
    m_art(art)
{
}

Song::Song(const Song& other) :
    QObject(),
    m_path(other.m_path),
    m_title(other.m_title),
    m_album(other.m_album),
    m_artist(other.m_artist),
    m_art(other.m_art) {
}

Song& Song::operator=(const Song& other) {
    m_path = other.m_path;
    m_title = other.m_title;
    m_album = other.m_album;
    m_artist = other.m_artist;
    m_art = other.m_art;
    return *this;
}

QString Song::path() const {
    return m_path;
}

QString Song::title() const {
    return m_title;
}

QString Song::album() const {
    return m_album;
}

QString Song::artist() const {
    return m_artist;
}

QString Song::art() const {
    return m_art;
}
