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
    ArtistObject();
    explicit ArtistObject(const QString &artist); 
    ArtistObject(const ArtistObject&);

    ArtistObject& operator=(const ArtistObject&);

    QString artist() const;
    QList<QObject*> getSong() const;

};

Q_DECLARE_METATYPE(ArtistObject)

#endif // ARTISTOBJECT

