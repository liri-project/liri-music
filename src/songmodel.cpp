#include "songmodel.h"
#include "musicdatabase.h"
#include "moc_songmodel.cpp"
#include <stdio.h>
#include <iostream>

SongModel::SongModel(QObject *parent):
    QAbstractListModel(parent)
{

}

int SongModel::rowCount(const QModelIndex &parent) const
{
    int result = 0;

    if(!parent.isValid())
        result = MusicDatabase::get().getAllSongs().size();

    return result;
}

QVariant SongModel::data(const QModelIndex &index, int role) const
{
    Song current = MusicDatabase::get().getAllSongs().at(index.row());
    switch(role)
    {
    case TitleRole:
        return QVariant::fromValue(current.title());
    case AlbumRole:
        return QVariant::fromValue(current.album());
    case PathRole:
        return QVariant::fromValue(current.path());
    case IdRole:
        return QVariant::fromValue(current.id());
    }
    return QVariant();

}

QList<QObject*> SongModel::getSongsByAlbum(int id) const {
    QList<QObject*> songList;
    QList<Song> current = MusicDatabase::get().getSongsByAlbum(id);

    for(const auto& song : current){
        songList.append(new Song(song.id(), song.path(), song.title(), song.album(), song.artist(), "placeholder"));
    }
    return songList;
}

QList<QObject*> SongModel::getAllSongs() const {
    QList<QObject*> songList;
    QList<Song> current = MusicDatabase::get().getAllSongs();

    for(const auto& song : current){
        songList.append(new Song(song.id(), song.path(), song.title(), song.album(), song.artist(), "placeholder"));
    }
    return songList;
}

QList<QObject*> SongModel::getSongsByArtist(int id) const {
    QList<QObject*> songList;
    QList<Song> current = MusicDatabase::get().getSongsByArtist(id);

    for(const auto& song : current){
        songList.append(new Song(song.id(), song.path(), song.title(), song.album(), song.artist(), "placeholder"));
    }
    return songList;
}

QString SongModel::getArtist(int id) const {
    QString artist = MusicDatabase::get().getArtist(id);
    std::cout << "Artist " << artist.toStdString() << std::endl;
    return artist;
}

QString SongModel::getAlbum(int id) const {
    QString album = MusicDatabase::get().getAlbum(id);
    return album;
}

QHash<int, QByteArray> SongModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[AlbumRole] = "album";
    roles[PathRole] = "path";
    roles[IdRole] = "id";
    return roles;
}
