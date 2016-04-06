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
            (exists, "SELECT COUNT(id) FROM Albums WHERE id = :id", (id))
        )
        ,
        ((id, quint64))
        ((title, QString))
        ((artist, QString))
        ((art, QString))
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

QList<Album> MusicDatabase::getAllAlbums() {
    return database::getAll<Album>(db);
}

QList<Artist> MusicDatabase::getAllArtists() {
    QList<Artist> artistList;
    QSqlQuery artistsQuery("SELECT * FROM Artists", db);

    while(artistsQuery.next()) {
        artistList.push_back(Artist { artistsQuery.value(1).toString() });
    }

    return artistList;
}

QList<Song> MusicDatabase::getAllSongs() {
    QList<Song> songList;

    QSqlQuery songsQuery("SELECT * FROM Songs");
    while(songsQuery.next()) {
        QString title = songsQuery.value(2).toString();
        QString path = songsQuery.value(1).toString();
        QString album = songsQuery.value(4).toString();
        QString artist = songsQuery.value(3).toString();
        QString art = songsQuery.value(5).toString();

        songList.push_back(Song { path, title, album, artist, art });
    }

    return songList;
}

void MusicDatabase::addArtist(const Artist& artist) {
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
    QSqlQuery songExistsQuery(QString("SELECT COUNT (*) FROM Songs WHERE artist = '%1' AND title = '%2'").arg(song.artist()).arg(song.title()), db);
    songExistsQuery.next();
    if(songExistsQuery.value(0).toInt() == 0)
    {
        QSqlQuery addSongQuery(db);
        addSongQuery.prepare("INSERT INTO Songs(path, title, artist, album, art) VALUES (:path, :title, :artist, :album, :art)");
        addSongQuery.bindValue(":path", song.path());
        addSongQuery.bindValue(":title", song.title());
        addSongQuery.bindValue(":artist", song.artist());
        addSongQuery.bindValue(":album", song.album());
        addSongQuery.bindValue(":art", song.art());
        addSongQuery.exec();
    }
}

void MusicDatabase::addAlbum(const Album &album)
{
    if(database::find(db, album).size() == 0) {
        std::cout << "Inserting album." << std::endl;
        QSqlQuery addAlbumQuery(db);
        addAlbumQuery.prepare("INSERT INTO Albums(album, artist, image) VALUES (:title, :artist, :art)");
        addAlbumQuery.bindValue(":title", album.title());
        addAlbumQuery.bindValue(":artist", album.artist());
        addAlbumQuery.bindValue(":art", album.art());
        addAlbumQuery.exec();
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
