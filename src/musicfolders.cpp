#include "musicfolders.h"
#include "moc_musicfolders.cpp"

#include <sstream>

#include <QFileInfo>
#include <QVariant>
#include <QtSql/QSqlQuery>
#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>

#include "albumobject.h"

QList<QObject*> MusicFolders::getAlbums() {
    return m_albums;
}

QString MusicFolders::initialMusicScan() {
    return m_initialMusicScan;
}

QString MusicFolders::getNotify() {
    return m_notify;
}

void MusicFolders::addSongsToDatabase(QDir dir,
                                      TagLib::String path,
                                      QString newpath,
                                      QString filename,
                                      QSqlDatabase db) {
    TagLib::FileRef f(path.toCString());
    TagLib::Tag *tag = f.tag();
    std::cout << "Adding " << tag->title() << " " << tag->artist() << " to the database." << std::endl;
    QString artist = tag->artist().toCString(true);
    QString title = tag->title().toCString(true);
    QString album = tag->album().toCString(true);

    if(db.open()) {
        QSqlQuery qry;
        qry.prepare("CREATE TABLE IF NOT EXISTS Songs(id INTEGER PRIMARY KEY AUTOINCREMENT, path TEXT, title TEXT, artist TEXT, album TEXT, art TEXT)");

        if(qry.exec()) {
        }

        QSqlQuery createArtist;
        createArtist.prepare("CREATE TABLE IF NOT EXISTS Artists(id INTEGER PRIMARY KEY AUTOINCREMENT, artist TEXT)");

        if(createArtist.exec()) {
        }

        QSqlQuery createAlbums;
        createAlbums.prepare("CREATE TABLE IF NOT EXISTS Albums(id INTEGER PRIMARY KEY AUTOINCREMENT, album TEXT, artist TEXT, art TEXT)");

        if(createAlbums.exec()) {
        }

        QSqlQuery getAlbum;
        getAlbum.prepare("select * from Albums where album='" + album + "'");
        if(getAlbum.exec()) {
            if(getAlbum.first()) {
            } else {
                QString art = "placeholder";
                bool recursive = true;
                bool symlinks = false;
                dir.setSorting(QDir::Name);
                QDir::Filters df = QDir::Files | QDir::NoDotAndDotDot;
                if(recursive) df |= QDir::Dirs;
                if(not symlinks) df |= QDir::NoSymLinks;
                QStringList qsl = dir.entryList(df, QDir::Name | QDir::DirsFirst);

                foreach(const QString& entry, qsl) {
                    QFileInfo finfo(dir, entry);

                    if(finfo.completeSuffix() == "jpg") {
                        art = finfo.absoluteFilePath();
                    }
                }

                QSqlQuery insertAlbum;
                insertAlbum.prepare("INSERT INTO Albums (id, album, artist, art) VALUES (NULL,'"+ album +"', '"+ artist +"', '"+ art + "')");
                if(insertAlbum.exec()) {
                }
            }
        }
    } else {
    }
}

void MusicFolders::setMusicScan(QDir d, bool recursive, bool symlinks ) {
    QSqlDatabase db = QSqlDatabase::database(QString("qt_sql_default_connection"), true);

    std::cout << "Doing music scan for " << std::endl;
    d.setSorting(QDir::Name);
    QDir::Filters df = QDir::Files | QDir::NoDotAndDotDot;
    if(recursive) df |= QDir::Dirs;
    if(not symlinks) df |= QDir::NoSymLinks;
    QStringList qsl = d.entryList(df, QDir::Name | QDir::DirsFirst);

    foreach(const QString& entry, qsl) {
        QFileInfo finfo(d, entry);
        std::cout << "File Info: " << finfo.absoluteFilePath().toStdString() << std::endl;

        if( finfo.isDir()) {
            QDir sd(finfo.absoluteFilePath());
        setMusicScan(sd);
        } else {
        if (finfo.completeSuffix().toStdString().find("mp3") != std::string::npos)
            addSongsToDatabase(finfo.dir(), finfo.absoluteFilePath().toStdString(), QString(finfo.absoluteFilePath()), finfo.fileName(), db);
        }
    }
    m_initialMusicScan = "Done";
}

void MusicFolders::setMusicFolder(const QString& folder) {
    QSqlDatabase db = QSqlDatabase::database(QString("qt_sql_default_connection"), true);

    db.setHostName("localhost");
    db.setDatabaseName("vinymusic");
    if(db.open()) {
        std::cout << "Music folder, " << folder.toStdString() << std::endl;

        QSqlQuery getFolder;
        getFolder.prepare("Select * FROM Settings where setting='folder'");
        if(getFolder.exec()) {
            if(getFolder.next()){
                QSqlQuery setFolder;
                setFolder.prepare("UPDATE Setetings SET value='"+ folder +"' WHERE setting='folder'");
            if(setFolder.exec()){
                m_getMusicFolder = folder;
            }
        }
        } else {
            QSqlQuery setFolder;
            setFolder.prepare("INSERT INTO Settings VALUES (NULL, 'folder', '"+folder+"')");
            if(setFolder.exec()){
                m_getMusicFolder = folder;
            }
        }
    }

    emit folderChanged();
}

void MusicFolders::initMusicScan(const QString& t) {
    QDir d = QDir::homePath() + QLatin1String("/Music");
    setMusicScan(d);
}

QString MusicFolders::getMusicFolder() const {
    return m_getMusicFolder;
}

QString MusicFolders::initialMusicScan() const {
    return m_initialMusicScan;
}

void MusicFolders::setNotify(const QString& title) {
    std::cout << title.toStdString() << std::endl;
    std::string notification = title.toStdString();
    std::stringstream ss;
    ss << "notify-send " << title.toStdString();
}

void MusicFolders::setAlbums(const QList<QObject*> &albums) {
    QList<QObject*> albumList;
    QSqlDatabase db = QSqlDatabase::database(QString("qt_sql_default_connection"),true);

    if(db.open()) {
        QSqlQuery getAllAlbums;
        getAllAlbums.prepare("select * FROM Albums");
        if(getAllAlbums.exec()) {
            while(getAllAlbums.next()) {
                QString album = getAllAlbums.value(1).toString();
                QString artist = getAllAlbums.value(2).toString();
                QString art = getAllAlbums.value(3).toString();
                albumList.append(new AlbumObject(album, artist, art));
            }
        }
        if(albumList.count() > 0) {
            m_albums = albumList;
        }else{
            albumList.append(new AlbumObject("undefined", "undefined", "undefined"));
            m_albums = albumList;
        }
    }
}

