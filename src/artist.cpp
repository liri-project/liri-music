#include "artist.h"
#include "moc_artist.cpp"

#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "song.h"

Artist::Artist() :
    QObject() {
}

Artist::Artist(quint64 id, const QString& name) :
    QObject(),
    m_name(name),
    m_id(id)
{
}

Artist::Artist(const Artist& other) :
    QObject(),
    m_name(other.m_name),
    m_id(other.m_id) {
}

Artist& Artist::operator=(const Artist& other) {
    m_name = other.m_name;
    m_id = other.m_id;
    return *this;
}

quint64 Artist::id() const {
    return m_id;
}

QString Artist::name() const {
    return m_name;
}

void Artist::setId(quint64 id) {
    m_id = id;
}

void Artist::setName(const QString& name) {
    m_name = name;
}
