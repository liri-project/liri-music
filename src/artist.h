#ifndef LIRI_MUSIC_ARTIST_H
#define LIRI_MUSIC_ARTIST_H

#include <QObject>
#include <QList>

class Artist : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(quint64 id READ id WRITE setId)

    QString m_name;
    quint64 m_id;
public:
    Artist();
    Artist(quint64, const QString &);
    Artist(const Artist&);

    Artist& operator=(const Artist&);

    quint64 id() const;
    QString name() const;

    void setId(quint64);
    void setName(const QString&);
};

Q_DECLARE_METATYPE(Artist)

#endif // ARTISTOBJECT

