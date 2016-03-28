#ifndef SONG_OBJECT_MAIN
#define SONG_OBJECT_MAIN

#include <QObject>

class SongObject : public QObject
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
    SongObject();
    explicit SongObject(const QString &path, const QString &title, const QString &album, const QString &artist, const QString &art); 
    SongObject(const SongObject&);

    SongObject& operator=(const SongObject&);

    QString path();
    QString title();
    QString album();
    QString artist();
    QString art();
};

Q_DECLARE_METATYPE(SongObject)

#endif // SONG_OBJECT_MAIN
