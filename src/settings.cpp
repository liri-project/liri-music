#include "settings.h"
#include "moc_settings.cpp"

Setting::Setting() :
    QObject() {
}

Setting::Setting(const Setting & setting) :
    QObject(),
    m_id(setting.m_id),
    m_name(setting.m_name),
    m_value(setting.m_value) {
}

Setting::Setting(quint64 id, const QString& name, const QString& value) :
    QObject(),
    m_id(id),
    m_name(name),
    m_value(value) {
}

Setting& Setting::operator=(const Setting& other) {
    m_id = other.m_id;
    m_name = other.m_name;
    m_value = other.m_value;
    return *this;
}

quint64 Setting::id() const {
    return m_id;
}

QString Setting::name() const {
    return m_name;
}

QString Setting::value() const {
    return m_value;
}

void Setting::setId(quint64 id) {
    m_id = id;
}

void Setting::setName(const QString& name) {
    m_name = name;
}

void Setting::setValue(const QString& value) {
    m_value = value;
}
