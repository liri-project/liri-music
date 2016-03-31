#include "musicscanner.h"
#include "moc_musicscanner.cpp"
#include "musicdatabase.h"

#include <QDir>
#include <QList>
#include <QGst/Caps>
#include <QGst/Discoverer>
#include <iostream>

MusicScanner::MusicScanner() {
}

void MusicScanner::startScan() {
    QDir musicDirectory { MusicDatabase::get().getMusicFolder() };
    QGst::DiscovererPtr discoverer = QGst::Discoverer::create(QGst::ClockTime::fromSeconds(1));
    scan(musicDirectory, discoverer);
}

void MusicScanner::scan(const QDir& dir, QGst::DiscovererPtr& discoverer) {
    QDir::Filters directoryFilters = QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs | QDir::NoSymLinks;

    QFileInfoList entries = dir.entryInfoList(directoryFilters, QDir::Name);

    for(const auto& entry : entries) {
        if(entry.isDir())
            scan(QDir { entry.absoluteFilePath() }, discoverer);
        else {
            QGst::DiscovererInfoPtr info = discoverer->discoverUri(QUrl::fromLocalFile(entry.absoluteFilePath()).toEncoded());
            SongObject song { info->uri().toLocalFile(), info->tags().title(), info->tags().tagValue("album").toString(), info->tags().artist(), "placeholder" };
            emit foundSong(song);
        }
    }
}
