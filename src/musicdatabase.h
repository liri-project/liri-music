#ifndef LIRI_MUSIC_MUSICDATABASE_H
#define LIRI_MUSIC_MUSICDATABASE_H

#include <QList>
#include <QtSql/QSqlDatabase>

#include <exception>

#include "album.h"
#include "artist.h"
#include "song.h"
#include "settings.h"

class MusicDatabase : public QObject {
    Q_OBJECT
public:
    class DatabaseNotFoundException : public std::exception {
    public:
        DatabaseNotFoundException() {}
        virtual ~DatabaseNotFoundException() noexcept { }

        virtual const char* what() const noexcept {
            return "Unable to open database.";
        }
    };

    static MusicDatabase& get();
public:
    QList<Album> getAllAlbums();
    QList<Artist> getAllArtists();
    QList<Song> getAllSongs();
    QList<Song> getSongsByAlbum(int id);
    QList<Song> getSongsByArtist(int id);
    QString getArtist(int id);
    QString getAlbum(int id);

    QString getMusicFolder();
    QByteArray getArt(const QString&);
public slots:
    void addSong(const Song&);
    void addAlbum(const Album&album);
    void addArtist(const Artist&);
    void setMusicFolder(const QString&);
    void addArtworkToAlbum(const Album&, QByteArray);
    void libraryItemFound(Artist, Song, Album, QByteArray);
signals:
    void musicFolderChanged(const QString& path);
    void addedNewArtist(Artist);
    void addedNewAlbum(Album);
    void addedNewSong(Song);
protected:
    MusicDatabase();
private:
    QSqlDatabase db;
};

#endif
