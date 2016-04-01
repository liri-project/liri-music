#include <QFileInfo>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QStandardPaths>
#include <QString>
#include <QThread>
#include <QGst/Init>
#include "album.h"
#include "albummodel.h"
#include "musicdatabase.h"
#include "musicscanner.h"
#include "albumartprovider.h"

int main(int argc, char *argv[]){
    qRegisterMetaType<Album>();
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

    AlbumModel albumModel;
    engine.rootContext()->setContextProperty("allSongObjects", QVariant::fromValue(MusicDatabase::get().getAllSongs()));
    engine.rootContext()->setContextProperty("allArtists", QVariant::fromValue(MusicDatabase::get().getAllArtists()));
    engine.rootContext()->setContextProperty("albumModel", &albumModel);
    engine.addImageProvider("art", new AlbumArtProvider());

    MusicScanner scanner {};
    MusicDatabase& db = MusicDatabase::get();
    QObject::connect(&scanner, &MusicScanner::foundSong, &db, &MusicDatabase::addSong);
    QObject::connect(&scanner, &MusicScanner::foundAlbum, &albumModel, &AlbumModel::addAlbum);
    QObject::connect(&scanner, &MusicScanner::foundAlbumArt, &db, &MusicDatabase::addArtworkToAlbum);

    QThread t;
    scanner.moveToThread(&t);
    QObject::connect(&t, &QThread::started, &scanner, &MusicScanner::startScan);
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, &scanner, &MusicScanner::stop);
    t.start();

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    int retval = app.exec();
    t.wait();
    return retval;
}


