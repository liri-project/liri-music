#ifndef ALBUMMODEL_H
#define ALBUMMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>

class AlbumModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AlbumRoles {
        TitleRole = Qt::UserRole + 1,
        ArtRole,
        ArtistRole
    };

    AlbumModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

protected:
    QHash<int, QByteArray> roleNames() const;
};

#endif
