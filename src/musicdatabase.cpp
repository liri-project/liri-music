#include "musicdatabase.h"
#include "moc_musicdatabase.cpp"

#include <QDir>
#include <QStandardPaths>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <iostream>
#include "database.h"

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
    database::makeTable<Setting>(db);
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
    QList<Setting> currentSettings = database::find_by<database::settings::setting>(db, "folder");
    if(currentSettings.size() == 0) {
        Setting setting { 0, "folder", QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first() };
        database::insert(db, setting);
        return setting.value();
    } else {
        return currentSettings.front().value();
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
