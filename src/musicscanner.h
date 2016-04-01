#ifndef LIRI_MUSIC_MUSICSCANNER_H
#define LIRI_MUSIC_MUSICSCANNER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QGst/Discoverer>
#include <QGlib/Connect>
#include "songobject.h"
#include "album.h"

class QDir;

class MusicScanner : public QObject {
    Q_OBJECT
public:
    MusicScanner();

public slots:
    void startScan();
    void directoryChanged(const QString &path);

signals:
    void foundSong(const SongObject&);
    void foundAlbum(const Album &);
    void foundAlbumArt(const Album&, QByteArray art);

private:
    void scan(const QDir&, QGst::DiscovererPtr&);
    QFileSystemWatcher watcher;
};

#endif
