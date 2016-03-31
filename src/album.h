#ifndef ALBUMOBJECT
#define ALBUMOBJECT
#include <QObject>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QList>
#include "songobject.h"
#include <QVariant>

class Album : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString artist READ artist CONSTANT)
    Q_PROPERTY(QString art READ art CONSTANT)
    Q_PROPERTY(QList<QObject*> getSong READ getSong CONSTANT)

    QString m_title;
    QString m_artist;
    QString m_art;
public:
    Album();
    explicit Album(const QString &title, const QString &artist, const QString &art);
    Album(const Album&);

    Album& operator=(const Album&);

    QString title() const;
    QString artist() const;
    QString art() const;
    QList<QObject*> getSong();

};

Q_DECLARE_METATYPE(Album);

#endif // ALBUMOBJECT

