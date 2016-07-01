#include "musicscanner.h"
#include "moc_musicscanner.cpp"
#include "musicdatabase.h"

#include <QDir>
#include <QList>
#include <QGst/Buffer>
#include <QGst/Caps>
#include <QGst/Discoverer>
#include <QGst/Sample>
#include <QThread>
#include <iostream>

MusicScanner::MusicScanner():
    watcher()
{
    QDir musicDirectory { MusicDatabase::get().getMusicFolder() };
    std::cout << musicDirectory.absolutePath().toStdString() << std::endl;
    watcher.addPath(musicDirectory.absolutePath());
    QObject::connect(&watcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryChanged(QString)));
}

void MusicScanner::startScan() {
    QDir musicDirectory { MusicDatabase::get().getMusicFolder() };
    QGst::DiscovererPtr discoverer = QGst::Discoverer::create(QGst::ClockTime::fromSeconds(1));
    scan(musicDirectory, discoverer);
}

void MusicScanner::directoryChanged(const QString &path)
{
    Q_UNUSED(path)
    startScan();
}

void MusicScanner::scan(const QDir& dir, QGst::DiscovererPtr& discoverer) {
    QDir::Filters directoryFilters = QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs | QDir::NoSymLinks;

    QFileInfoList entries = dir.entryInfoList(directoryFilters, QDir::Name);

    for(const auto& entry : entries) {
        if(entry.isDir())
        {
            watcher.addPath(entry.absolutePath());
            scan(QDir { entry.absoluteFilePath() }, discoverer);
        }
        else {
            QGst::DiscovererInfoPtr info;

            try {
                info = discoverer->discoverUri(QUrl::fromLocalFile(entry.absoluteFilePath()).toEncoded());
            } catch (QGlib::Error& e) {
                continue;
            }

            if(info->audioStreams().count() != 0){

                Artist artist;
                if(!info->tags().artist().isEmpty())
                  artist = Artist { 0, info->tags().artist() };
                else
                  artist = Artist { 0, "Unknown Artist" };

                Song song;
                if(!info->tags().title().isEmpty())
                  song = Song { 0, info->uri().toLocalFile(), info->tags().title(), 0, 0, "placeholder" };
                else {
                  QString path = MusicDatabase::get().getMusicFolder();
                  QString title = info->uri().toLocalFile().right(info->uri().toLocalFile().size() - path.size() - 1);
                  song = Song { 0, info->uri().toLocalFile(), title, 0, 0, "placeholder" };
                }

                Album album;

                if(!info->tags().tagValue("album").toString().isEmpty())
                  album = Album { 0, info->tags().tagValue("album").toString(), 0, "placeholder" };
                else
                  album = Album { 0, "Unknown Album", 0, "placeholder" };

                //emit foundSong(song);
                //emit foundAlbum(album);

                QByteArray artwork;

                if(info->tags().image()) {
                    QGst::BufferPtr buffer = info->tags().image()->buffer();
                    // This is only necessary to keep the compile warning free
                    // Why QByteArray doesn't take a std::size_t is beyond me
                    int size = buffer->size() & std::numeric_limits<int>::max();
                    //QByteArray outputBuffer { size, 0 };
                    artwork.resize(size);
                    buffer->extract(0, artwork.data(), buffer->size());
                    //emit foundAlbumArt(album, outputBuffer);
                }

                emit foundLibraryItem(artist, song, album, artwork);
            }
        }
    }
}

void MusicScanner::stop() {
  this->thread()->quit();
}

