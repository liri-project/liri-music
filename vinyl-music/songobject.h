#ifndef SONGOBJECT_H
#define SONGOBJECT_H

#include <QObject>
#include <qobject.h>
#include <qmetaobject.h>
#include <qmetatype.h>
class SongObject : public QObject
{
    Q_OBJECT

    //Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    //Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
public:
    explicit SongObject(QString name, QString color, QObject *parent = 0);

public:

QString name() const;
void setName(const QString &newName);
QString color() const;
void setColor(const QString &newName);

signals:
void nameChanged();
void colorChanged();

private:
QString m_name;
QString m_color;




};


/*
class SongObject : public QObject
{
Q_OBJECT

Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)

public:
SongObject(const QString &name, const QString &color);

QString name() const;
void setName(const QString &newName);
QString color() const;
void setColor(const QString &newName);

signals:
void nameChanged();
void colorChanged();

private:
QString m_name;
QString m_color;

};
*/



#endif // SONGOBJECT_H
