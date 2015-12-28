/****************************************************************************
** Meta object code from reading C++ file 'musicfolders.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../liri-vinyl/musicfolders.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'musicfolders.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MusicFolders_t {
    QByteArrayData data[6];
    char stringdata0[77];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MusicFolders_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MusicFolders_t qt_meta_stringdata_MusicFolders = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MusicFolders"
QT_MOC_LITERAL(1, 13, 13), // "folderChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 16), // "musicScanChanged"
QT_MOC_LITERAL(4, 45, 14), // "getMusicFolder"
QT_MOC_LITERAL(5, 60, 16) // "initialMusicScan"

    },
    "MusicFolders\0folderChanged\0\0"
    "musicScanChanged\0getMusicFolder\0"
    "initialMusicScan"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MusicFolders[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       2,   26, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    0,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       4, QMetaType::QString, 0x00495003,
       5, QMetaType::QString, 0x00495003,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void MusicFolders::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MusicFolders *_t = static_cast<MusicFolders *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->folderChanged(); break;
        case 1: _t->musicScanChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MusicFolders::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MusicFolders::folderChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (MusicFolders::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MusicFolders::musicScanChanged)) {
                *result = 1;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        MusicFolders *_t = static_cast<MusicFolders *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getMusicFolder(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->initialMusicScan(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        MusicFolders *_t = static_cast<MusicFolders *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setMusicFolder(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->initMusicScan(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject MusicFolders::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MusicFolders.data,
      qt_meta_data_MusicFolders,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MusicFolders::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MusicFolders::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MusicFolders.stringdata0))
        return static_cast<void*>(const_cast< MusicFolders*>(this));
    return QObject::qt_metacast(_clname);
}

int MusicFolders::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void MusicFolders::folderChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void MusicFolders::musicScanChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
