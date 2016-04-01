#ifndef LIRI_MUSIC_MUSICDATABASE_H
#define LIRI_MUSIC_MUSICDATABASE_H

#include <QList>
#include <QtSql/QSqlDatabase>

#include <exception>

#include "album.h"
#include "artistobject.h"
#include "songobject.h"

class MusicDatabase : public QObject {
    Q_OBJECT
public:
    class DatabaseNotFoundException : public std::exception {
    public:
        DatabaseNotFoundException() {}
        virtual ~DatabaseNotFoundException() noexcept { }

        virtual const char* what() const noexcept {
            return "Unable to open database.";
        };
    };

    static MusicDatabase& get();
public:
    QList<Album> getAllAlbums();
    QList<ArtistObject> getAllArtists();
    QList<SongObject> getAllSongs();

    QString getMusicFolder();
public slots:
    void addSong(const SongObject&);
    void addAlbum(const Album&album);
    void addArtist(const ArtistObject&);
    void setMusicFolder(const QString&);
    void addArtworkToAlbum(const Album&, QByteArray);
signals:
    void musicFolderChanged(const QString& path);
protected:
    MusicDatabase();
private:
    QSqlDatabase db;
};

#endif
