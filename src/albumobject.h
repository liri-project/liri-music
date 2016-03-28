#ifndef ALBUMOBJECT
#define ALBUMOBJECT
#include <QObject>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QList>
#include "songobject.h"
#include <QVariant>

class AllAlbums : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString getAlbums READ getAlbums WRITE setAlbums NOTIFY albumsChanged)

public:
    void setAlbums(const QString &album){
        m_getAlbums = album;

        emit albumsChanged();
    }



    QString getAlbums() const {
        return m_getAlbums;
    }



signals:
    void albumsChanged();
private:
    QString m_getAlbums;
};

class Message : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
public:
    void setAuthor(const QString &a) {
        if (a != m_author) {
            m_author = a;
            emit authorChanged();
        }
    }
    QString author() const {
        return m_author;
    }
signals:
    void authorChanged();
private:
    QString m_author;
};

class AlbumObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString artist READ artist CONSTANT)
    Q_PROPERTY(QString art READ art CONSTANT)
    Q_PROPERTY(QList<QObject*> getSong READ getSong CONSTANT)

    QString m_title;
    QString m_artist;
    QString m_art;
public:
    AlbumObject();
    explicit AlbumObject(const QString &title, const QString &artist, const QString &art);
    AlbumObject(const AlbumObject&);

    AlbumObject& operator=(const AlbumObject&);

    QString title();
    QString artist();
    QString art();
    QList<QObject*> getSong();
};

Q_DECLARE_METATYPE(AlbumObject);

#endif // ALBUMOBJECT

