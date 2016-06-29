#include "album.h"
#include "moc_album.cpp"

Album::Album() :
    QObject() {
}

Album::Album(quint64 id, const QString& title, quint64 artist, const QString& art) :
    QObject(), m_title(title), m_artist(artist), m_art(art), m_id(id) {
}

Album::Album(const Album& other) :
    QObject(), m_title(other.m_title), m_artist(other.m_artist), m_art(other.m_art), m_id(other.m_id) {
}

Album& Album::operator=(const Album& other) {
    m_id = other.m_id;
    m_title = other.m_title;
    m_artist = other.m_artist;
    m_art = other.m_art;
    return *this;
}

quint64 Album::id() const {
    return m_id;
}

QString Album::title() const {
    return m_title;
}

quint64 Album::artist() const {
    return m_artist;
}

QString Album::art() const {
    return m_art;
}

void Album::setId(quint64 id) {
    m_id = id;
}

void Album::setTitle(const QString &title) {
    m_title = title;
}

void Album::setArtist(quint64 artist) {
    m_artist = artist;
}

void Album::setArt(const QString &art) {
    m_art = art;
}
