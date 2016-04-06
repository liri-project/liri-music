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
    Q_PROPERTY(QString artist READ artist WRITE setArtist)
    Q_PROPERTY(QString art READ art WRITE setArt)
    Q_PROPERTY(QList<QObject*> getSong READ getSong)

    QString m_title;
    QString m_artist;
    QString m_art;
    quint64 m_id;
public:
    Album();
    explicit Album(quint64 id, const QString &title, const QString &artist, const QString &art);
    Album(const Album&);

    Album& operator=(const Album&);

    quint64 id() const;
    QString title() const;
    QString artist() const;
    QString art() const;

    void setId(quint64);
    void setTitle(const QString&);
    void setArtist(const QString&);
    void setArt(const QString&);

    QList<QObject*> getSong();

};

Q_DECLARE_METATYPE(Album);

#endif // ALBUMOBJECT

