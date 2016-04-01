#ifndef LIRI_MUSIC_SONG_H
#define LIRI_MUSIC_SONG_H

#include <QObject>

class Song : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString path READ path CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString album READ album CONSTANT)
    Q_PROPERTY(QString artist READ artist CONSTANT)
    Q_PROPERTY(QString art READ art CONSTANT)
    QString m_path;
    QString m_title;
    QString m_album;
    QString m_artist;
    QString m_art;

public:
    Song();
    explicit Song(const QString &path, const QString &title, const QString &album, const QString &artist, const QString &art); 
    Song(const Song&);

    Song& operator=(const Song&);

    QString path() const;
    QString title() const;
    QString album() const;
    QString artist() const;
    QString art() const;
};

Q_DECLARE_METATYPE(Song)

#endif // SONG_OBJECT_MAIN
