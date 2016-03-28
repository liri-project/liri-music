#include "musicdatabase.h"
#include "moc_musicdatabase.cpp"

#include <QDir>
#include <QStandardPaths>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

MusicDatabase::MusicDatabase() 
    : db { QSqlDatabase::addDatabase("QSQLITE") } {
    db.setDatabaseName(QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0] +
        QDir::separator() + "musicdb.sqlite");
    if(!db.open())
        throw DatabaseNotFoundException {};

    if(!db.tables().contains(QLatin1String("Albums"))) {
        QSqlQuery createAlbums {
            "CREATE TABLE IF NOT EXISTS Albums(id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "album TEXT, artist TEXT, art TEXT)", db };
    }

    if(!db.tables().contains(QLatin1String("Songs"))) {
        QSqlQuery createSongs {
            "CREATE TABLE IF NOT EXISTS Songs(id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "path TEXT, title TEXT, artist TEXT, album TEXT, art TEXT)", db };
    }

    if(!db.tables().contains(QLatin1String("Artists"))) {
        QSqlQuery createArtists {
            "CREATE TABLE IF NOT EXISTS Artists(id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "artist TEXT", db };
    }

    if(!db.tables().contains(QLatin1String("Settings"))) {
        QSqlQuery createSettings {
            "CREATE TABLE IF NOT EXISTS Settings(id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "setting TEXT, value TEXT)", db };
    }
}

MusicDatabase& MusicDatabase::get() {
    static MusicDatabase musicDatabase {};
    return musicDatabase;
}

QList<AlbumObject> MusicDatabase::getAllAlbums() {
    QList<AlbumObject> albumList;

    QSqlQuery albumsQuery("SELECT * FROM Albums", db);

    while(albumsQuery.next()) {
        QString album = albumsQuery.value(1).toString();
        QString artist = albumsQuery.value(2).toString();
        QString art = albumsQuery.value(3).toString();
        albumList.push_back(AlbumObject {album, artist, art});
    }
    return albumList;
}

QList<ArtistObject> MusicDatabase::getAllArtists() {
    QList<ArtistObject> artistList;
    QSqlQuery artistsQuery("SELECT * FROM Artists", db);

    while(artistsQuery.next()) {
        artistList.push_back(ArtistObject { artistsQuery.value(1).toString() });
    }

    return artistList;
}

QList<SongObject> MusicDatabase::getAllSongs() {
    QList<SongObject> songList;
 
    QSqlQuery songsQuery("SELECT * FROM Songs");
    while(songsQuery.next()) {
        QString title = songsQuery.value(2).toString();
        QString path = songsQuery.value(1).toString();
        QString album = songsQuery.value(4).toString();
        QString artist = songsQuery.value(3).toString();
        QString art = songsQuery.value(5).toString();

        songList.push_back(SongObject { path, title, album, artist, art });
    }

    return songList;
}

void MusicDatabase::addArtist(const ArtistObject& artist) {
    QSqlQuery artistExistsQuery;
    artistExistsQuery.prepare("SELECT COUNT(artist) AS artistCount WHERE artist = :artist");
    artistExistsQuery.bindValue(":artist", artist.artist());
    artistExistsQuery.exec();
    artistExistsQuery.next();
    if(artistExistsQuery.value(1).toInt() == 0) {
        QSqlQuery artistQuery;
        artistQuery.prepare("INSERT INTO Artists (artist) VALUES(:artist)");
        artistQuery.bindValue(":artist", artist.artist());
        artistQuery.exec();
    }
}

QString MusicDatabase::getMusicFolder() {
    QSqlQuery getFolderQuery;
    getFolderQuery.prepare("SELECT * FROM Settings where setting='folder'");
    getFolderQuery.exec();
    if(getFolderQuery.first()) {
        return getFolderQuery.value(3).toString();
    } else {
        QSqlQuery createFolderQuery;
        QString initialFolder = QDir::homePath() + QLatin1String("/Music");
        createFolderQuery.prepare("INSERT INTO Settings(setting, value) VALUES('folder', :value)");
        createFolderQuery.bindValue(":value", initialFolder);
        createFolderQuery.exec();
        return initialFolder;
    }
}

void MusicDatabase::setMusicFolder(const QString& folder) {
    QSqlQuery setFolderQuery;
    setFolderQuery.prepare("UPDATE Settings(value) SET value = :value WHERE setting='folder'");
    setFolderQuery.bindValue(":value", folder);
    setFolderQuery.exec();
    emit musicFolderChanged(folder);
}