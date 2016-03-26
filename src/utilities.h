#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QDir>
#include <QtSql/QSql>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QList>
#include "songobject.h"
#include <QVariant>
#include <iostream>
#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/taglib_config.h>
#include <taglib/tpropertymap.h>
#include <qdebug.h>
#include <qqml.h>
#include <QQuickView>
#include <QDateTime>
#include <QQmlContext>
#include <QFileInfo>

class Utilities : public QThread
{
    Q_OBJECT
public:
    explicit Utilities(QObject *parent = 0);
    void run();
    void setMusicScan(QDir, bool, bool);
    void addSongsToDatabase(QDir, TagLib::String, QString, QString, QSqlDatabase);


signals:
    void musicScanChanged();
    void allAlbumschanged();

public slots:
    void onAlbumsChanged();
};

#endif // UTILITIES_H
