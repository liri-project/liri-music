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
#include "albumobject.h"
#include "artistobject.h"
#include <QStandardPaths>
#include "songobject.h"
#include <unistd.h>
#include "musicfolders.h"
#include "utilities.h"
#include "musicdatabase.h"
#include "musicscanner.h"
#include <QGst/Init>

int main(int argc, char *argv[]){
    QGuiApplication app(argc, argv);
    app.setApplicationName("LiriMusic");
    QQmlApplicationEngine engine;

    QGst::init();
    if (app.arguments().size() > 1) {
        // If Vinyl Music was opened with a song path:
        QFileInfo finfo;
        finfo.setFile(app.arguments().at(1));
        engine.rootContext()->setContextProperty("loadedFileFolder", QString(finfo.path()));
    }

    QString musicLocation = MusicDatabase::get().getMusicFolder();
    QString stream_directory = musicLocation + QLatin1String("/streams");


    // Create path variables accessible in QML:
    engine.rootContext()->setContextProperty("homeDirectory", musicLocation);
    engine.rootContext()->setContextProperty("streamDirectory", stream_directory);

    engine.rootContext()->setContextProperty("allSongObjects", QVariant::fromValue(MusicDatabase::get().getAllSongs()));
    engine.rootContext()->setContextProperty("allArtists", QVariant::fromValue(MusicDatabase::get().getAllArtists()));
    engine.rootContext()->setContextProperty("allAlbums", QVariant::fromValue(MusicDatabase::get().getAllAlbums()));

    MusicScanner scanner {};
    MusicDatabase& db = MusicDatabase::get();
    QObject::connect(&scanner, &MusicScanner::foundSong, &db, &MusicDatabase::addSong);

    QThread t;
    scanner.moveToThread(&t);
    QObject::connect(&t, &QThread::started, &scanner, &MusicScanner::startScan);
    t.start();

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}


