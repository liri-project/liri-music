#include "utilities.h"
#include <QObject>
#include <QDebug>
#include <QDir>
#include <QtSql/QSql>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QList>
#include <main.h>
#include <QVariant>
#include <iostream>
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
#include <musicfolders.h>

Utilities::Utilities(QObject *parent)
{

}

void Utilities::addSongsToDatabase(QDir dir, TagLib::String path, QString newpath, QString filename, QSqlDatabase db){
    // Get TagLib instance of song by path
    TagLib::FileRef f(path.toCString());
    TagLib::Tag *tag = f.tag();
    std::cout << "Adding " << tag->title() << " " << tag->artist() << " to the database." << std::endl;
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

void Utilities::setMusicScan(QDir d, bool recursive=true, bool symlinks=false ) {
    QSqlDatabase db = QSqlDatabase::database(QString("qt_sql_default_connection"),true);

    std::cout << "Doing music scan for " << std::endl;
    // Get top directory (~/Music by default), and recursively scan for mp3
    d.setSorting( QDir::Name );
    QDir::Filters df = QDir::Files | QDir::NoDotAndDotDot;
    if (recursive) df |= QDir::Dirs;
    if (not symlinks) df |= QDir::NoSymLinks;
    QStringList qsl = d.entryList(df, QDir::Name | QDir::DirsFirst);

    // For each: if folder, run this method, if mp3, add to DB
    foreach (const QString &entry, qsl) {
        QFileInfo finfo(d, entry);
        std::cout << "File Info: " << finfo.absoluteFilePath().toStdString() << std::endl;
        //std::cout << "Suffix: " << finfo.completeSuffix().toStdString() << std::endl;
        if ( finfo.isDir() ) {
            QDir sd(finfo.absoluteFilePath());
            this->setMusicScan(sd, true, false);
        } else {
            if (finfo.completeSuffix().toStdString().find("mp3") != std::string::npos || finfo.completeSuffix().toStdString().find("MP3") != std::string::npos)
                this->addSongsToDatabase(finfo.dir(), finfo.absoluteFilePath().toStdString(), QString(finfo.absoluteFilePath()), finfo.fileName(), db);
        }
        //emit allAlbumsChanged();

    }


}

void Utilities::run(){
    qDebug() << "Running thread";
    QDir d = QDir::homePath() + QLatin1String("/Music");
    this->setMusicScan(d);


}

void Utilities::onAlbumsChanged(){
    std::cout << "Finished running thread" << std::endl;
}
