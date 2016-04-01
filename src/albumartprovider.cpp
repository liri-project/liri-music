#include "albumartprovider.h"
#include "musicdatabase.h"
#include <QFileInfo>
#include <QImage>

AlbumArtProvider::AlbumArtProvider()
    : QQuickImageProvider { QQuickImageProvider::Image } {
}

QImage AlbumArtProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize) {
    auto& db = MusicDatabase::get();

    QByteArray artwork = db.getArt(id);

    if(artwork.size() != 0) {
        return QImage::fromData(artwork);
    } else {
        return QImage { "qrc://images/placeholder.png" };
    }
}
