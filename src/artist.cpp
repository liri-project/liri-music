#include "artist.h"
#include "moc_artist.cpp"

#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "song.h"

Artist::Artist() :
    QObject() {
}

Artist::Artist(quint64 id, const QString& artist) :
    QObject(),
    m_artist(artist),
    m_id(id)
{
}

Artist::Artist(const Artist& other) :
    QObject(),
    m_artist(other.m_artist),
    m_id(other.m_id) {
}

Artist& Artist::operator=(const Artist& other) {
    m_artist = other.m_artist;
    m_id = other.m_id;
    return *this;
}

quint64 Artist::id() const {
    return m_id;
}

QString Artist::artist() const {
    return m_artist;
}

void Artist::setId(quint64 id) {
    m_id = id;
}

void Artist::setArtist(const QString& artist) {
    m_artist = artist;
}
