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
        std::cout << song.title().toStdString() << std::endl;
        songList.append(new Song(song.id(), song.path(), song.title(), song.album(), song.artist(), "placeholder"));
    }
    /*

        if(db.open()){
            QSqlQuery getAllAlbums;
            getAllAlbums.prepare("select * FROM Albums");
            if(getAllAlbums.exec()){
                while(getAllAlbums.next()){
                    QString album = getAllAlbums.value(1).toString();
                    QString artist = getAllAlbums.value(2).toString();
                    QString art = getAllAlbums.value(3).toString();
                    albumList.append(new AlbumObject(album, artist, art));

                }
            }
        }
        if(albumList.count() > 0){
            return albumList;
        }else{
            albumList.append(new AlbumObject("undefined", "undefined", "undefined"));
            return albumList;
        }

    QList<Song> songList = current;
    return songList;
    */
    return songList;

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
