#include "musicdatabase.h"
#include "moc_musicdatabase.cpp"

#include <QDir>
#include <QStandardPaths>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "database.h"
#include <iostream>

CREATE_TABLE(
    Album, albums, "Albums",
    (
        (create, "CREATE TABLE IF NOT EXISTS Albums(id INTEGER PRIMARY KEY AUTOINCREMENT, album TEXT, artist TEXT, image BLOB)"),
        (find, "SELECT * FROM Albums WHERE album = :album AND artist = :artist", (title, artist)),
        (find_all, "SELECT * FROM Albums"),
        (exists, "SELECT COUNT(id) FROM Albums WHERE id = :id", (id)),
        (insert, "INSERT INTO Albums(album, artist, image) VALUES(:album, :artist, :image)", (title, artist, art))
    )
    ,
    ((id, quint64))
    ((title, QString))
    ((artist, QString))
    ((art, QString))
)

CREATE_TABLE(
    Song, songs, "Songs",
    (
        (create, "CREATE TABLE IF NOT EXISTS Songs(id INTEGER PRIMARY KEY AUTOINCREMENT, path TEXT, title TEXT, artist TEXT, album TEXT, art TEXT)"),
        (find, "SELECT * FROM Songs WHERE title = :title AND artist = :artist AND album = :album", (title, artist, album)),
        (find_all, "SELECT * FROM Songs"),
        (exists, "SELECT COUNT(id) FROM Songs WHERE id = :id", (id)),
        (insert, "INSERT INTO Songs(path, title, artist, album, art) VALUES(:path, :title, :artist, :album)", (path, title, artist, album))
    ),
    ((id, quint64))
    ((path, QString))
    ((title, QString))
    ((artist, QString))
    ((album, QString))
)

CREATE_TABLE(
    Artist, artists, "Artists",
    (
        (create, "CREATE TABLE IF NOT EXISTS Artists(id INTEGER PRIMARY KEY AUTOINCREMENT, artist TEXT"),
        (find, "SELECT * FROM Artists WHERE artist = :artist", (artist)),
        (find_all, "SELECT * FROM Artists"),
        (exists, "SELECT COUNT(id) FROM Artists WHERE id = :id", (id)),
        (insert, "INSERT INTO Artists(artist) VALUES(:artist)", (artist))
    ),
    ((id, quint64))
    ((artist, QString))
)

MusicDatabase::MusicDatabase()
    : db { QSqlDatabase::addDatabase("QSQLITE") } {
    QDir databaseDirectory { QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0] };
    if(!databaseDirectory.exists()) {
        databaseDirectory.mkpath(".");
    }
    db.setDatabaseName(QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0] +
        QDir::separator() + "musicdb.sqlite");
    if(!db.open())
        throw DatabaseNotFoundException {};

    database::makeTable<Album>(db);
    database::makeTable<Song>(db);
    database::makeTable<Artist>(db);

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

QList<Album> MusicDatabase::getAllAlbums() {
    return database::findAll<Album>(db);
}

QList<Artist> MusicDatabase::getAllArtists() {
    return database::findAll<Artist>(db);
}

QList<Song> MusicDatabase::getAllSongs() {
    return database::findAll<Song>(db);
}

void MusicDatabase::addArtist(const Artist& artist) {
    if(database::find(db, artist).size() == 0) {
        database::insert(db, artist);
    }
}

QString MusicDatabase::getMusicFolder() {
    QSqlQuery getFolderQuery;
    getFolderQuery.prepare("SELECT * FROM Settings where setting='folder'");
    getFolderQuery.exec();
    if(getFolderQuery.first()) {
        return getFolderQuery.value(2).toString();
    } else {
        QSqlQuery createFolderQuery;
        QString initialFolder = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();
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

void MusicDatabase::addSong(const Song& song) {

    addAlbum(Album{ 0, song.album(), song.artist(), song.art()});
    //Check if the songs already exists in DB.
    if(database::find(db, song).size() == 0)
        database::insert(db, song);
}

void MusicDatabase::addAlbum(const Album &album)
{
    if(database::find(db, album).size() == 0) {
        database::insert(db, album);
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
