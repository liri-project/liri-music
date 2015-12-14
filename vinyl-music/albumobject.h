#ifndef ALBUMOBJECT
#define ALBUMOBJECT
#include <QObject>
#include <QtSql/QSql>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QList>
#include <main.h>
#include <QVariant>

class AlbumObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString artist READ artist CONSTANT)
    Q_PROPERTY(QString art READ art CONSTANT)
    Q_PROPERTY(QList<QObject*> getSong READ getSong CONSTANT)

    QString m_title;
    QString m_artist;
    QString m_art;

    QString title() {return m_title;}
    QString artist() {return m_artist;}
    QString art() {return m_art;}
    QList<QObject*> getSong(){
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("localhost");
        db.setDatabaseName("vinylmusic");
        QList<QObject*> songList;

        if(db.open()){
            // DB is open; lets get all songs
            QSqlQuery getSongs;
            getSongs.prepare("Select * FROM Songs where album='"+ m_title +"'");
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
    explicit AlbumObject(const QString &title, const QString &artist, const QString &art) :
        QObject(),
        m_title(title),
        m_artist(artist),
        m_art(art)
    {}

};

#endif // ALBUMOBJECT

