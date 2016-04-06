#include "settings.h"
#include "moc_settings.cpp"

Setting::Setting() :
    QObject() {
}

Setting::Setting(const Setting & setting) :
    QObject(),
    m_id(setting.m_id),
    m_setting(setting.m_setting),
    m_value(setting.m_value) {
}

Setting::Setting(quint64 id, const QString& setting, const QString& value) :
    QObject(),
    m_id(id),
    m_setting(setting),
    m_value(value) {
}

Setting& Setting::operator=(const Setting& other) {
    m_id = other.m_id;
    m_setting = other.m_setting;
    m_value = other.m_value;
    return *this;
}

quint64 Setting::id() const {
    return m_id;
}

QString Setting::setting() const {
    return m_setting;
}

QString Setting::value() const {
    return m_value;
}

void Setting::setId(quint64 id) {
    m_id = id;
}

void Setting::setSetting(const QString& setting) {
    m_setting = setting;
}

void Setting::setValue(const QString& value) {
    m_value = value;
}
