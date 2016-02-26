#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlPropertyMap>
#include <QString>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/taglib_config.h>
#include <taglib/tpropertymap.h>
#include <qdebug.h>
#include <qqml.h>
#include <QQuickView>
#include <QDateTime>
#include <QQmlContext>
#include <QFileInfo>
#include <QDir>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QList>
#include <QObject>
#include <albumobject.h>
#include <artistobject.h>
#include <QStandardPaths>
#include <main.h>
#include <unistd.h>
#include <musicfolders.h>
#include <utilities.h>



QList<QObject*> getAlbums(QSqlDatabase db){
    QList<QObject*> albumList;

    if(db.open()){
        QSqlQuery getAllAlbums;
        getAllAlbums.prepare("select * FROM Albums");
        if(getAllAlbums.exec()){
            while(getAllAlbums.next()){
                QString album = getAllAlbums.value(1).toString();
                QString artist = getAllAlbums.value(2).toString();
                QString art = getAllAlbums.value(3).toString();
                albumList.append(new AlbumObject(album, artist, art));

            }
        }
    }
    if(albumList.count() > 0){
        return albumList;
    }else{
        albumList.append(new AlbumObject("undefined", "undefined", "undefined"));
        return albumList;
    }
}

QList<QObject*> getArtists(QSqlDatabase db){
    QList<QObject*> artistList;
    if(db.open()){
        QSqlQuery getArtists;
        getArtists.prepare("select * FROM Artists");
        if(getArtists.exec()){
            while(getArtists.next()){
                QString artist = getArtists.value(1).toString();
                artistList.append(new ArtistObject(artist));

            }
        }
    }
    if(artistList.count() > 0){
        return artistList;
    }else{
        artistList.append(new ArtistObject("undefined"));
        return artistList;
    }

}


QList<QObject*> getAllSongs(QSqlDatabase db){
    QList<QObject*> songList;

    if(db.open()){
        // DB is open; lets get all songs
        QSqlQuery getSongs;
        getSongs.prepare("Select * FROM Songs");
        if(getSongs.exec()){
            while(getSongs.next()){

                QString title = getSongs.value(2).toString();
                QString path = getSongs.value(1).toString();
                QString album = getSongs.value(4).toString();
                QString artist = getSongs.value(3).toString();
                QString art = getSongs.value(5).toString();

                //std::cout << art.toStdString() << std::endl;
                songList.append(new SongObject(path, QString::fromStdString(title.toStdString()), album, artist, art));
                //songList.append(new SongObject(getSongs.value(2).fromValue, getSongs.value(1).fromValue, getSongs.value(4).fromValue, getSongs.value(3).fromValue));
            }
        }
    }


    if(songList.count() > 0){
        songList[0]->setObjectName("allSongObjects");
        return songList;
    }else{
        songList.append(new SongObject("undefined", "undefined", "undefined", "undefined", "undefined"));
        songList[0]->setObjectName("allSongObjects");
        return songList;
    }
}

void addSongsToDatabase(QDir dir, TagLib::String path, QString newpath, QString filename, QSqlDatabase db){
    // Get TagLib instance of song by path
    TagLib::FileRef f(path.toCString());
    TagLib::Tag *tag = f.tag();
    // std::cout << "Adding " << tag->title() << " " << tag->artist() << " to the database." << std::endl;
    QString artist = tag->artist().toCString(true);
    QString title = tag->title().toCString(true);
    QString album = tag->album().toCString(true);
    //QString art = tag->

    if(db.open()){
        QSqlQuery qry;
        qry.prepare("CREATE TABLE IF NOT EXISTS Songs(id INTEGER PRIMARY KEY AUTOINCREMENT, path TEXT, title TEXT, artist TEXT, album TEXT, art TEXT)");

        if(qry.exec()){
            //std::cout << "Created table" << std::endl;
        }

        QSqlQuery createArtist;
        createArtist.prepare("CREATE TABLE IF NOT EXISTS Artists(id INTEGER PRIMARY KEY AUTOINCREMENT, artist TEXT)");

        if(createArtist.exec()){
            // We created the table if it didnt exist... move along -->
        }

        QSqlQuery createAlbums;
        createAlbums.prepare("CREATE TABLE IF NOT EXISTS Albums(id INTEGER PRIMARY KEY AUTOINCREMENT, album TEXT, artist TEXT, art TEXT)");

        if(createAlbums.exec()){
            // We created the table if it didnt exist... move along -->
        }

        // Check if item already exists for this path
        QSqlQuery getAlbum;
        getAlbum.prepare("select * from Albums where album='" + album + "'");
        if(getAlbum.exec()){
            if(getAlbum.first()){

            }else{
                QString art = "placeholder";
                bool recursive=true;
                bool symlinks=false;
                dir.setSorting( QDir::Name );
                QDir::Filters df = QDir::Files | QDir::NoDotAndDotDot;
                if (recursive) df |= QDir::Dirs;
                if (not symlinks) df |= QDir::NoSymLinks;
                QStringList qsl = dir.entryList(df, QDir::Name | QDir::DirsFirst);

                // For each: if folder, run this method, if mp3, add to DB
                foreach (const QString &entry, qsl) {
                    QFileInfo finfo(dir, entry);

                     if (finfo.completeSuffix()=="jpg"){
                            art = finfo.absoluteFilePath();
                     }

                }

                QSqlQuery insertAlbum;
                insertAlbum.prepare("INSERT INTO Albums (id, album, artist, art) VALUES (NULL,'"+ album +"', '"+ artist +"', '"+ art +"')");
                if(insertAlbum.exec()){
                    // We inserted the album  :)
                }
            }
        }



        // Check if item already exists for this path
        QSqlQuery getArtist;
        getArtist.prepare("select * from Artists where artist='" + artist + "'");
        if(getArtist.exec()){
            if(getArtist.first()){

            }else{

                QSqlQuery insertArtist;
                insertArtist.prepare("INSERT INTO Artists (id, artist) VALUES (NULL,'"+ artist +"')");
                if(insertArtist.exec()){
                    // We inserted the album  :)
                }
            }
        }

        QSqlQuery getSong;
        getSong.prepare("SELECT * FROM Songs WHERE path='" + newpath + "'");
        if(getSong.exec()){
            // If .first == true, it already exists
           if(getSong.first()){
                while(getSong.next()){
                    //std::cout << getSong.value(1).toString().toUtf8().constData() << std::endl;
                }
            }else{
               QString art = "placeholder";
               bool recursive=true;
               bool symlinks=false;
               dir.setSorting( QDir::Name );
               QDir::Filters df = QDir::Files | QDir::NoDotAndDotDot;
               if (recursive) df |= QDir::Dirs;
               if (not symlinks) df |= QDir::NoSymLinks;
               QStringList qsl = dir.entryList(df, QDir::Name | QDir::DirsFirst);

               // For each: if folder, run this method, if mp3, add to DB
               foreach (const QString &entry, qsl) {
                   QFileInfo finfo(dir, entry);

                    if (finfo.completeSuffix()=="jpg" || finfo.completeSuffix()=="png"){
                           art = finfo.absoluteFilePath();
                    }

               }
                // If not, we add it.
                QSqlQuery insertqry;
                if(tag->title().isEmpty()){
                    std::cout << "Empty title"
;                    title = filename;
                }
                insertqry.prepare("INSERT INTO Songs (id, path, title, artist, album, art) VALUES "
                                  "(NULL, '"+ newpath +"', '"+ title +"', '"+ artist +"', '"+ album +"', '"+art+"')");
                if(insertqry.exec()){
                    std::cout << "Inserted data successfully: " << title.toStdString() << " " << std::endl;
                }else{
                     std::cout << "Data not inserted" << std::endl;
                }
            }
        }


    }else{
        // If we couldn't open the database it means the entire program is broken and will need a rewrite
        //std::cout << "Couldnt open database" << std::endl;
    }
}

bool firstMusicScan(QDir d, QSqlDatabase db, bool recursive=true, bool symlinks=false ) {
    // Get top directory (~/Music by default), and recursively scan for mp3
    d.setSorting( QDir::Name );
    QDir::Filters df = QDir::Files | QDir::NoDotAndDotDot;
    if (recursive) df |= QDir::Dirs;
    if (not symlinks) df |= QDir::NoSymLinks;
    QStringList qsl = d.entryList(df, QDir::Name | QDir::DirsFirst);

    // For each: if folder, run this method, if mp3, add to DB
    foreach (const QString &entry, qsl) {
        QFileInfo finfo(d, entry);
        //std::cout << "File Info: " << finfo.absoluteFilePath().toStdString() << std::endl;
        //std::cout << "Suffix: " << finfo.completeSuffix().toStdString() << std::endl;
        if ( finfo.isDir() ) {
            QDir sd(finfo.absoluteFilePath());
            firstMusicScan(sd, db);
        } else {
            if (finfo.completeSuffix().toStdString().find("mp3") != std::string::npos)
                addSongsToDatabase(finfo.dir(), finfo.absoluteFilePath().toStdString(), QString(finfo.absoluteFilePath()), finfo.fileName(), db);
        }
    }
    if(d.count() > 0){
        return false;
    }else{
        return true;
    }
}

bool initialQuery(QSqlDatabase db){
    // Open DB connection

    std::cout << "Loading database data. " << std::endl;

    if(db.open()){
        // Create Settings table if it isnt there
        QSqlQuery qry;
        qry.prepare("CREATE TABLE IF NOT EXISTS Settings(id INTEGER PRIMARY KEY AUTOINCREMENT, setting TEXT, value TEXT)");
        if(qry.exec()){
            std::cout << "Created table" << std::endl;

            // DB is open; let
            QSqlQuery getFolder;
            getFolder.prepare("Select * FROM Settings where setting='folder'");
            if(getFolder.exec()){

                if(getFolder.first()){
                 // Music folder is already set.
                    std::cout << "Executed getFolder" << std::endl;

                }else{
                    std::cout << "Music folder not set" << std::endl;
                    QSqlQuery setFolder;
                    QString initialFolder = QDir::homePath() + QLatin1String("/Music");
                    setFolder.prepare("INSERT INTO Settings VALUES (NULL, 'folder', '"+ initialFolder +"')");
                    if(setFolder.exec()){
                        // Set initial folder
                    }
                }

            }else{
                std::cout << "Couldnt get the music folder" << std::endl;

            }
        }else{
            std::cout << "Didnt create table" << std::endl;
        }
    }else{
        std::cout << "Couldnt open database" << std::endl;
    }
    return true;
}

int main(int argc, char *argv[]){

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    if (app.arguments().size() > 1) {
        // If Vinyl Music was opened with a song path:
        QFileInfo finfo;
        finfo.setFile(app.arguments().at(1));
        engine.rootContext()->setContextProperty("loadedFileFolder", QString(finfo.path()));
    }

    // Initialize the database if not exists, and set Settings table


    const QStringList musicLocations = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
    QString musicLocation = musicLocations.isEmpty() ?
                QDir::homePath() + QLatin1String("/Music") : musicLocations.first();

    // Open DB and perform initial music dir scan/build Songs table
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("vinylmusic");
    std::cout << "Loading database data. " << db.connectionName().toStdString() << std::endl;
    if(initialQuery(db)){

    }

    std::cout << "Loaded" << std::endl;

    QString stream_directory = musicLocation + QLatin1String("/streams");


    // Create path variables accessible in QML:
    engine.rootContext()->setContextProperty("homeDirectory", musicLocation);
    engine.rootContext()->setContextProperty("streamDirectory", stream_directory);

    std::cout << "Song count" << QString(getAllSongs(db).count()).toStdString() << std::endl;

    Message msg;
    engine.rootContext()->setContextProperty("msg", &msg);

    AllAlbums aa;
    engine.rootContext()->setContextProperty("aa", &aa);


    engine.rootContext()->setContextProperty("allSongObjects", QVariant::fromValue(getAllSongs(db)));
    engine.rootContext()->setContextProperty("allArtists", QVariant::fromValue(getArtists(db)));
    engine.rootContext()->setContextProperty("allAlbums", QVariant::fromValue(getAlbums(db)));
    //db.close();

    MusicFolders defaultFolders;


    Utilities newUtils;
    QObject::connect(&newUtils,SIGNAL(allAlbumschanged()),&defaultFolders,SLOT(onAlbumsChanged()));
    newUtils.start();


    engine.rootContext()->setContextProperty("musicFolder", &defaultFolders);

    // Create view from main.qml:
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    // Run the app
    return app.exec();
}


