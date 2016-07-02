#include "albummodel.h"
#include "musicdatabase.h"
#include "moc_albummodel.cpp"
#include <stdio.h>
#include <iostream>

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
    Album current = MusicDatabase::get().getAllAlbums().at(index.row());
    switch(role)
    {
    case TitleRole:
        return QVariant::fromValue(current.title());
    case ArtRole:
        return QVariant::fromValue(current.art());
    case ArtistRole:
        return QVariant::fromValue(current.artist());
    case IdRole:
        return QVariant::fromValue(current.id());
    }
    return QVariant();

}

QList<Song> AlbumModel::getSingleAlbum(int id, int role) const {
    QList<Song> current = MusicDatabase::get().getSongsByAlbum(id);

    for(const auto& song : current)
      std::cout << song.title().toStdString() << std::endl;

    return current;
}


void AlbumModel::addAlbum(const Album &album)
{
    emit beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    MusicDatabase::get().addAlbum(album);
    std::cout << "album is " << album.title().toStdString() << std::endl;
    emit endInsertRows();
}

QHash<int, QByteArray> AlbumModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[ArtRole] = "art";
    roles[ArtistRole] = "artist";
    roles[IdRole] = "id";
    return roles;
}
