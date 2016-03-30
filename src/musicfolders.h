#ifndef MUSICFOLDERS_H
#define MUSICFOLDERS_H

#include <QDir>
#include <QString>
#include <QObject>
#include <QList>
#include <QtSql/QSqlDatabase>
#include <taglib/tstring.h>

class MusicFolders : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString getMusicFolder READ getMusicFolder WRITE setMusicFolder NOTIFY folderChanged)
    Q_PROPERTY(QString initialMusicScan READ initialMusicScan WRITE initMusicScan NOTIFY musicScanChanged)
    Q_PROPERTY(QString notify READ getNotify WRITE setNotify NOTIFY notifyChanged)
    Q_PROPERTY(QList<QObject*> getAlbums READ getAlbums WRITE setAlbums NOTIFY allAlbumsChanged)
public:
    QList<QObject*> getAlbums();
    QString initialMusicScan();
    QString getNotify();
    void addSongsToDatabase(QDir dir, TagLib::String path, QString newpath, QString filename, QSqlDatabase db);

    void setMusicScan(QDir d, bool recursive=true, bool symlinks=false );
    void setMusicFolder(const QString &folder);

    void initMusicScan(const QString &t);

    QString getMusicFolder() const;

    QString initialMusicScan() const;

    void setNotify(const QString &title);

    void setAlbums(const QList<QObject*> &albums);

signals:
    void folderChanged();
    void musicScanChanged();
    void notifyChanged();
    void allAlbumsChanged();

private:
    QString m_getMusicFolder;
    QString m_initialMusicScan;
    QString m_notify;
    QList<QObject*> m_albums;

public slots:


};
#endif // MUSICFOLDERS_H
