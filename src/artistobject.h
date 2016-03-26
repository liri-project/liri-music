#ifndef ARTISTOBJECT
#define ARTISTOBJECT
#include <QObject>
#include <QtSql/QSql>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QList>
#include "songobject.h"
#include <QVariant>

class ArtistObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString artist READ artist CONSTANT)
    Q_PROPERTY(QList<QObject*> getSong READ getSong CONSTANT)

    QString m_artist;

    QString artist() {return m_artist;}
    QList<QObject*> getSong(){
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("localhost");
        db.setDatabaseName("vinylmusic");
        QList<QObject*> songList;

        if(db.open()){
            // DB is open; lets get all songs
            QSqlQuery getSongs;
            getSongs.prepare("Select * FROM Songs where artist='"+ m_artist +"'");
            if(getSongs.exec()){
                while(getSongs.next()){

                    QString title = getSongs.value(2).toString();
                    QString path = getSongs.value(1).toString();
                    QString album = getSongs.value(4).toString();
                    QString artist = getSongs.value(3).toString();
                    QString art = getSongs.value(5).toString();

                    //std::cout << title.toStdString() << std::endl;
                    songList.append(new SongObject(path, QString::fromStdString(title.toStdString()), album, artist, art));
                    //songList.append(new SongObject(getSongs.value(2).fromValue, getSongs.value(1).fromValue, getSongs.value(4).fromValue, getSongs.value(3).fromValue));
                }
            }
        }

        return songList;
        }

public:
   explicit ArtistObject(const QString &artist) :
        QObject(),
        m_artist(artist)
    {}

};

#endif // ARTISTOBJECT

