#ifndef LIRI_MUSIC_MUSICSCANNER_H
#define LIRI_MUSIC_MUSICSCANNER_H

#include <QObject>
#include <QGst/Discoverer>
#include <QGlib/Connect>
#include "songobject.h"

class QDir;

class MusicScanner : public QObject {
    Q_OBJECT
public:
    MusicScanner();

public slots:
    void startScan();

signals:
    void foundSong(const SongObject&);
private:
    void scan(const QDir&, QGst::DiscovererPtr&);
};

#endif
