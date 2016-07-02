#ifndef SONGMODEL_H
#define SONGMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QList>
#include <QString>

#include "song.h"

class SongModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SongRoles {
        TitleRole = Qt::UserRole + 1,
        AlbumRole,
        PathRole,
        IdRole
    };

    SongModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QList<QObject*> getSongsByAlbum(int id) const;
    Q_INVOKABLE QList<QObject*> getSongsByArtist(int id) const;
    Q_INVOKABLE QString getArtist(int id) const;
    Q_INVOKABLE QString getAlbum(int id) const;
    Q_INVOKABLE QList<QObject*> getAllSongs() const;

public slots:

protected:
    QHash<int, QByteArray> roleNames() const;
};

#endif
