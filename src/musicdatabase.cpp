#include "musicdatabase.h"
#include "moc_musicdatabase.cpp"

#include <QDir>
#include <QStandardPaths>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <iostream>
#include "database.h"
#include "schema.h"

MusicDatabase::MusicDatabase()
    : db { QSqlDatabase::addDatabase("QSQLITE") } {
    QDir databaseDirectory { QStandardPaths::standardLocations(
                    QStandardPaths::AppLocalDataLocation)[0] };
    if(!databaseDirectory.exists()) {
        databaseDirectory.mkpath(".");
    }
    db.setDatabaseName(QStandardPaths::standardLocations(
                           QStandardPaths::AppLocalDataLocation)[0] +
        QDir::separator() + "musicdb.sqlite");
    if(!db.open())
        throw DatabaseNotFoundException {};

    database::create<Album>(db);
    database::create<Song>(db);
    database::create<Artist>(db);
    database::create<Setting>(db);
}

MusicDatabase& MusicDatabase::get() {
    static MusicDatabase musicDatabase {};
    return musicDatabase;
}

QList<Album> MusicDatabase::getAllAlbums() {
    return database::find<Album>(db);
}

QList<Song> MusicDatabase::getSongsByAlbum(int id){
    return database::find<song::album>(db, id);
}

QList<Song> MusicDatabase::getSongsByArtist(int id){
    return database::find<song::artist>(db, id);
}

QList<Artist> MusicDatabase::getAllArtists() {
    return database::find<Artist>(db);
}

QString MusicDatabase::getArtist(int id){
    QList<Artist> artists = database::find<artist::id>(db, id);
    QString currentArtist;

    for(const auto& artist : artists){
        currentArtist = artist.name();

    }
    return currentArtist;

}

QString MusicDatabase::getAlbum(int id){
    QList<Album> albums = database::find<album::id>(db, id);
    QString currentAlbum = albums[0].title();
    return currentAlbum;


}

QList<Song> MusicDatabase::getAllSongs() {
    return database::find<Song>(db);
}

void MusicDatabase::addArtist(const Artist& artist) {
    if(database::find<artist::name>(db, artist.name()).size() == 0) {
        database::insert(db, artist);
    }
}

QString MusicDatabase::getMusicFolder() {
    QList<Setting> currentSettings = database::find<setting::name>(db, "folder");
    if(currentSettings.size() == 0) {
        Setting setting { 0, "folder",
                          QStandardPaths::standardLocations(
                                      QStandardPaths::MusicLocation).first() };
        database::insert(db, setting);
        return setting.value();
    } else {
        return currentSettings.front().value();
    }
}

void MusicDatabase::setMusicFolder(const QString& folder) {
    QSqlQuery setFolderQuery;
    setFolderQuery.prepare("UPDATE Settings(value) SET value = :value WHERE name='folder'");
    setFolderQuery.bindValue(":value", folder);
    setFolderQuery.exec();
    emit musicFolderChanged(folder);
}

void MusicDatabase::addSong(const Song& song) {
    //Check if the songs already exists in DB.
    if(database::find<song::title, song::artist, song::album>(db, song.title(), song.artist(), song.album()).size() == 0) {
        database::insert(db, song);
    }
}

void MusicDatabase::addAlbum(const Album &album) {
    if(database::find<album::title, album::artist>(db, album.title(), album.artist()).size() == 0) {
        database::insert(db, album);
    }
}

void MusicDatabase::libraryItemFound(Artist artist, Song song, Album album, QByteArray artwork) {
    // Try to insert the artist
    QList<Artist> artists = database::find<artist::name>(db, artist.name());
    if(artists.size() == 0) {
        // We didn't find one, so insert it
        database::insert(db, artist);
        artists = database::find<artist::name>(db, artist.name());
        emit addedNewArtist(artists.front());
    }

    // Try to insert the Album
    album.setArtist(artists.front().id());

    QList<Album> albums = database::find<album::title, album::artist>(db, album.title(), album.artist());
    if(albums.size() == 0) {
        // Once again, not found, so insert it
        database::insert(db, album);
        albums = database::find<album::title, album::artist>(db, album.title(), album.artist());
        emit addedNewAlbum(albums.front());
    }

    // We can finally try to insert the song
    song.setArtist(artists.front().id());
    song.setAlbum(albums.front().id());
    QList<Song> songs = database::find<song::title, song::artist, song::album>(db, song.title(), song.artist(), song.album());

    if(songs.size() == 0) {
        database::insert(db, song);
        songs = database::find<song::title, song::artist, song::album>(db, song.title(), song.artist(), song.album());
        emit addedNewSong(songs.front());
    }
}

void MusicDatabase::addArtworkToAlbum(const Album& album, QByteArray artwork) {
    QSqlQuery addArtQuery;
    addArtQuery.prepare("UPDATE Albums SET image = :image WHERE (artist = :artist AND album = :album)");
    addArtQuery.bindValue(":artist", album.artist());
    addArtQuery.bindValue(":album", album.title());
    addArtQuery.bindValue(":image", artwork);
    addArtQuery.exec();
}

QByteArray MusicDatabase::getArt(const QString& art) {
    QSqlQuery getArtQuery;
    getArtQuery.prepare("SELECT image FROM Albums WHERE art = :art");
    getArtQuery.bindValue(":art", art);
    getArtQuery.exec();
    getArtQuery.first();
    return getArtQuery.value(0).toByteArray();
}
