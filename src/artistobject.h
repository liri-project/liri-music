#ifndef ARTISTOBJECT
#define ARTISTOBJECT

#include <QObject>
#include <QList>

class ArtistObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString artist READ artist CONSTANT)
    Q_PROPERTY(QList<QObject*> getSong READ getSong CONSTANT)

    QString m_artist;

public:
    explicit ArtistObject(const QString &artist); 
    QString artist() const;
    QList<QObject*> getSong() const;

};

#endif // ARTISTOBJECT

