#ifndef SONG_OBJECT_MAIN
#define SONG_OBJECT_MAIN

#include <QObject>

class SongObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString color READ color CONSTANT)

    QString m_name;
    QString m_color;

    QString name() { return m_name;}
    QString color() { return m_color;}

public:
    explicit SongObject(const QString &name, const QString &color) :
        QObject(),
        m_name(name),
        m_color(color)
    {}

};

#endif // SONG_OBJECT_MAIN
