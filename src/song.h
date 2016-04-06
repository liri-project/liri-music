#ifndef LIRI_MUSIC_SONG_H
#define LIRI_MUSIC_SONG_H

#include <QObject>

class Song : public QObject
{
    Q_OBJECT

    Q_PROPERTY(quint64 id READ id WRITE setId)
    Q_PROPERTY(QString path READ path WRITE setPath)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString album READ album WRITE setAlbum)
    Q_PROPERTY(QString artist READ artist WRITE setArtist)
    Q_PROPERTY(QString art READ art WRITE setArt)

    QString m_path;
    QString m_title;
    QString m_album;
    QString m_artist;
    QString m_art;
    quint64 m_id;

public:
    Song();
    explicit Song(quint64, const QString &path, const QString &title, const QString &album, const QString &artist, const QString &art);
    Song(const Song&);

    Song& operator=(const Song&);

    QString path() const;
    QString title() const;
    QString album() const;
    QString artist() const;
    QString art() const;
    quint64 id() const;

    void setId(quint64);
    void setPath(const QString& path);
    void setTitle(const QString& title);
    void setAlbum(const QString& album);
    void setArtist(const QString& artist);
    void setArt(const QString& art);
};

Q_DECLARE_METATYPE(Song)

#endif // SONG_OBJECT_MAIN
