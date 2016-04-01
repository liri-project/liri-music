#ifndef LIRI_MUSIC_ALBUM_ART_PROVIDER_H
#define LIRI_MUSIC_ALBUM_ART_PROVIDER_H

#include <QQuickImageProvider>

class AlbumArtProvider : public QQuickImageProvider {
public:
    AlbumArtProvider();

    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;
};

#endif
