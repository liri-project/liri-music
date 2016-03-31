#include "songobject.h"
#include "moc_songobject.cpp"

SongObject::SongObject() :
    QObject() {
}

SongObject::SongObject(const QString& path, const QString& title, const QString& album,
                       const QString& artist, const QString& art) :
    QObject(),
    m_path(path),
    m_title(title),
    m_album(album),
    m_artist(artist),
    m_art(art)
{
}

SongObject::SongObject(const SongObject& other) :
    QObject(),
    m_path(other.m_path),
    m_title(other.m_title),
    m_album(other.m_album),
    m_artist(other.m_artist),
    m_art(other.m_art) {
}

SongObject& SongObject::operator=(const SongObject& other) {
    m_path = other.m_path;
    m_title = other.m_title;
    m_album = other.m_album;
    m_artist = other.m_artist;
    m_art = other.m_art;
    return *this;
}

QString SongObject::path() const {
    return m_path;
}

QString SongObject::title() const {
    return m_title;
}

QString SongObject::album() const {
    return m_album;
}

QString SongObject::artist() const {
    return m_artist;
}

QString SongObject::art() const {
    return m_art;
}
