#ifndef LIRI_MUSIC_SETTINGS_H
#define LIRI_MUSIC_SETTINGS_H

#include <QObject>

class Setting : public QObject {
    Q_OBJECT

    Q_PROPERTY(quint64 id READ id WRITE setId)
    Q_PROPERTY(QString setting READ setting WRITE setSetting)
    Q_PROPERTY(QString value READ value WRITE setValue)

    quint64 m_id;
    QString m_setting;
    QString m_value;
public:
    Setting();
    Setting(quint64 id, const QString& setting, const QString& value);
    Setting(const Setting&);

    Setting& operator=(const Setting& other);

    quint64 id() const;
    QString setting() const;
    QString value() const;

    void setId(quint64);
    void setSetting(const QString&);
    void setValue(const QString&);
};

#endif
