#ifndef LIRI_MUSIC_ARTIST_H
#define LIRI_MUSIC_ARTIST_H

#include <QObject>
#include <QList>

class Artist : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString artist READ artist CONSTANT)
    Q_PROPERTY(QList<QObject*> getSong READ getSong CONSTANT)

    QString m_artist;

public:
    Artist();
    explicit Artist(const QString &artist); 
    Artist(const Artist&);

    Artist& operator=(const Artist&);

    QString artist() const;
    QList<QObject*> getSong() const;

};

Q_DECLARE_METATYPE(Artist)

#endif // ARTISTOBJECT

