#ifndef ALBUMOBJECT
#define ALBUMOBJECT
#include <QObject>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QList>
#include "song.h"
#include <QVariant>

class Album : public QObject {
    Q_OBJECT

    Q_PROPERTY(quint64 id READ id WRITE setId)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(quint64 artist READ artist WRITE setArtist)
    Q_PROPERTY(QString art READ art WRITE setArt)

    QString m_title;
    quint64 m_artist;
    QString m_art;
    quint64 m_id;
public:
    Album();
    explicit Album(quint64 id, const QString &title, quint64 artist, const QString &art);
    Album(const Album&);

    Album& operator=(const Album&);

    quint64 id() const;
    QString title() const;
    quint64 artist() const;
    QString art() const;

    void setId(quint64);
    void setTitle(const QString&);
    void setArtist(quint64);
    void setArt(const QString&);
};

Q_DECLARE_METATYPE(Album);

#endif // ALBUMOBJECT

