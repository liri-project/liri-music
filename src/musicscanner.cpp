#include "musicscanner.h"
#include "moc_musicscanner.cpp"
#include "musicdatabase.h"

#include <QDir>
#include <QList>
#include <QGst/Caps>
#include <QGst/Discoverer>
#include <iostream>

MusicScanner::MusicScanner() {
    discoverer = QGst::Discoverer::create(QGst::ClockTime::fromSeconds(15));
    discoverer->start();
    QGlib::connect(discoverer, "discovered", this, &MusicScanner::uriDiscovered);
}

void MusicScanner::startScan() {
    QDir musicDirectory { MusicDatabase::get().getMusicFolder() };
    scan(musicDirectory);
}

void MusicScanner::scan(const QDir& dir) {
    QDir::Filters directoryFilters = QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs | QDir::NoSymLinks;

    QFileInfoList entries = dir.entryInfoList(directoryFilters, QDir::Name);

    for(const auto& entry : entries) {
        if(entry.isDir())
            scan(QDir { entry.absoluteFilePath() });
        else {
            discoverer->discoverUriAsync(QUrl::fromLocalFile(entry.absoluteFilePath()).toEncoded());
        }
    }
}

void MusicScanner::uriDiscovered(QGst::DiscovererInfoPtr ptr) {
    /*for(const auto& stream : ptr->streams()) {
        std::cout << stream->streamTypeNick().toStdString() << std::endl;
        std::cout << stream->caps()->toString().toStdString() << std::endl;
    }*/
    //std::cout << ptr->tags().title().toStdString() << std::endl;
    std::cout << "SongObject { " << std::endl;
    std::cout << "  path:" << ptr->uri().toLocalFile().toStdString() << std::endl;
    std::cout << "}" << std::endl;
    SongObject object { ptr->uri().toLocalFile(), ptr->tags().title(), ptr->tags().tagValue("album").toString(), ptr->tags().artist(), "" };
    emit foundSong(object);
}
