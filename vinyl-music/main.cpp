#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDeclarative/QDeclarativePropertyMap>
#include <QString>
#include <iostream>
#include <iomanip>
#include <stdio.h>
//#include </usr/local/lib/pkgconfig/taglib.pc>
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

void addSongsToDatabase(TagLib::String path, QString newpath, QSqlDatabase db){
    TagLib::FileRef f(path.toCString());
    TagLib::Tag *tag = f.tag();
    std::cout << "Adding " << tag->title() << " " << tag->artist() << " to the database." << std::endl;
    QString artist = tag->artist().toCString();
    QString title = tag->title().toCString();
    QString album = tag->album().toCString();

    if(db.open()){
        QSqlQuery qry;
        qry.prepare("CREATE TABLE IF NOT EXISTS Songs(id INTEGER PRIMARY KEY AUTOINCREMENT, path TEXT, title TEXT, artist TEXT, album TEXT)");

        if(qry.exec()){
            std::cout << "Created table" << std::endl;
        }else{
            std::cout << "Didnt create table" << std::endl;
        }

        QSqlQuery getSong;
        getSong.prepare("SELECT * FROM Songs WHERE path='" + newpath + "'");
        if(getSong.exec()){
           if(getSong.first()){
            while(getSong.next()){
                std::cout << getSong.value(1).toString().toUtf8().constData() << std::endl;
            }
        }else{
            QSqlQuery insertqry;
            insertqry.prepare("INSERT INTO Songs (id, path, title, artist, album) VALUES "
                              "(NULL, '"+ newpath +"', '"+ title +"', '"+ artist +"', '"+ album +"')");
            if(insertqry.exec()){
                std::cout << "Inserted data successfully" << std::endl;
            }else{
                 std::cout << "Data not inserted" << std::endl;
            }

        }
        }


        }else{
            std::cout << "Couldnt open database" << std::endl;
        }

    }

void recurseAddDir(QDir d, QSqlDatabase db, bool recursive=true, bool symlinks=false ) {
    d.setSorting( QDir::Name );
    QDir::Filters df = QDir::Files | QDir::NoDotAndDotDot;
    if (recursive) df |= QDir::Dirs;
    if (not symlinks) df |= QDir::NoSymLinks;
    QStringList qsl = d.entryList(df, QDir::Name | QDir::DirsFirst);
    foreach (const QString &entry, qsl) {
        QFileInfo finfo(d, entry);
        if ( finfo.isDir() ) {
            QDir sd(finfo.absoluteFilePath());
            recurseAddDir(sd, db);
        } else {
            if (finfo.completeSuffix()=="mp3")
                addSongsToDatabase(finfo.absoluteFilePath().toStdString(), QString(finfo.absoluteFilePath()), db);
        }
    }
}

void initialQuery(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("vinylmusic");
    std::cout << "Loading database data. " << std::endl;

    if(db.open()){
        qDebug() << "DB: DB is open";
        QSqlQuery qry;
        qry.prepare("CREATE TABLE IF NOT EXISTS Settings(id INTEGER PRIMARY KEY AUTOINCREMENT, setting TEXT, value TEXT)");

        if(qry.exec()){
            std::cout << "Created table" << std::endl;
        }else{
            std::cout << "Didnt create table" << std::endl;
        }

        /*
        QSqlQuery insertqry;
        insertqry.prepare("INSERT INTO Settings (id, setting, value) VALUES (NULL, 'test', 'testdata')");
        if(insertqry.exec()){
            std::cout << "Inserted data successfully" << std::endl;
        }else{
             std::cout << "Data not inserted" << std::endl;
        }
        */

        }else{
            std::cout << "Couldnt open database" << std::endl;
        }

        QSqlQuery getSettings;
        getSettings.prepare("Select * FROM Settings");
        if(getSettings.exec()){
            while(getSettings.next()){
                std::cout << getSettings.value(1).toString().toUtf8().constData() << std::endl;
            }
        }
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    if (argv[1]) {
        // If Vinyl Music was opened with a song path:
        QFileInfo finfo;
        finfo.setFile(argv[1]);


        engine.rootContext()->setContextProperty("loadedFileFolder", QString(finfo.path()));
    }

    initialQuery();





    TagLib::FileRef f("/home/nick/Music/Final Fantasy VIII OST/1 - 02 Balamb Garden.mp3");
    TagLib::Tag *tag = f.tag();
    std::cout << "title   - \"" << tag->artist()   << "\"" << std::endl;
    QString artist = tag->artist().toCString();
    engine.rootContext()->setContextProperty("loadedArtist", artist);

    // Get ~/Music directory and ~/Music/streams directory
    std::string home_directory = QDir::homePath().toStdString() + std::string("/Music");
    std::string stream_directory = QDir::homePath().toStdString() + std::string("/Music/streams");


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("vinylmusic");
    std::cout << "Loading database data. " << std::endl;

    recurseAddDir(QDir::homePath() + "/Music", db);

    // Create custom variables accessible to QML:
    engine.rootContext()->setContextProperty("homeDirectory", QString::fromStdString(home_directory));
    engine.rootContext()->setContextProperty("streamDirectory", QString::fromStdString(stream_directory));

    // Create view from main.qml:
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    // Run the app
    return app.exec();
}


