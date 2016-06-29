#ifndef LIRI_MUSIC_SETTINGS_H
#define LIRI_MUSIC_SETTINGS_H

#include <QObject>

class Setting : public QObject {
    Q_OBJECT

    Q_PROPERTY(quint64 id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString value READ value WRITE setValue)

    quint64 m_id;
    QString m_name;
    QString m_value;
public:
    Setting();
    Setting(quint64 id, const QString& name, const QString& value);
    Setting(const Setting&);

    Setting& operator=(const Setting& other);

    quint64 id() const;
    QString name() const;
    QString value() const;

    void setId(quint64);
    void setName(const QString&);
    void setValue(const QString&);
};

#endif
