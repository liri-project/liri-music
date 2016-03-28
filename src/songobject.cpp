#include "songobject.h"
#include "moc_songobject.cpp"

SongObject::SongObject(const QString& path, const QString& title, const QString& album,
                       const QString& artist, const QString& art) :
    m_path(path),
    m_title(title),
    m_album(album),
    m_artist(artist),
    m_art(art)
{
}

QString SongObject::path() {
    return m_path;
}

QString SongObject::title() {
    return m_title;
}

QString SongObject::album() {
    return m_album;
}

QString SongObject::artist() {
    return m_artist;
}

QString SongObject::art() {
    return m_art;
}
