#include "albummodel.h"
#include "musicdatabase.h"

AlbumModel::AlbumModel(QObject *parent):
    QAbstractListModel(parent)
{

}

int AlbumModel::rowCount(const QModelIndex &parent) const
{
    int result = 0;

    if(!parent.isValid())
        result = MusicDatabase::get().getAllAlbums().size();

    return result;
}

QVariant AlbumModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    Album current = MusicDatabase::get().getAllAlbums().at(index.row());
    switch(role)
    {
    case TitleRole:
        return QVariant::fromValue(current.title());
    case ArtRole:
        return QVariant::fromValue(current.art());
    case ArtistRole:
        return QVariant::fromValue(current.artist());
    }

    return QVariant();

}

QHash<int, QByteArray> AlbumModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[ArtRole] = "art";
    roles[ArtistRole] = "artist";
    return roles;
}
