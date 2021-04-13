/****************************************************************************
** Meta object code from reading C++ file 'gameMode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Gomoku/gameMode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gameMode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GameMode_t {
    QByteArrayData data[9];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GameMode_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GameMode_t qt_meta_stringdata_GameMode = {
    {
QT_MOC_LITERAL(0, 0, 8), // "GameMode"
QT_MOC_LITERAL(1, 9, 11), // "listenError"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 9), // "gameStart"
QT_MOC_LITERAL(4, 32, 12), // "MsgGameReady"
QT_MOC_LITERAL(5, 45, 10), // "PlayerRole"
QT_MOC_LITERAL(6, 56, 7), // "MsgChat"
QT_MOC_LITERAL(7, 64, 10), // "PlayerJoin"
QT_MOC_LITERAL(8, 75, 13) // "setPlayerRole"

    },
    "GameMode\0listenError\0\0gameStart\0"
    "MsgGameReady\0PlayerRole\0MsgChat\0"
    "PlayerJoin\0setPlayerRole"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GameMode[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    1,   46,    2, 0x06 /* Public */,
       6,    1,   49,    2, 0x06 /* Public */,
       7,    0,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    2,

       0        // eod
};

void GameMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GameMode *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->listenError(); break;
        case 1: _t->gameStart(); break;
        case 2: _t->MsgGameReady((*reinterpret_cast< PlayerRole(*)>(_a[1]))); break;
        case 3: _t->MsgChat((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->PlayerJoin(); break;
        case 5: _t->setPlayerRole((*reinterpret_cast< PlayerRole(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GameMode::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GameMode::listenError)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GameMode::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GameMode::gameStart)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GameMode::*)(PlayerRole );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GameMode::MsgGameReady)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (GameMode::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GameMode::MsgChat)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (GameMode::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GameMode::PlayerJoin)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GameMode::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_GameMode.data,
    qt_meta_data_GameMode,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GameMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameMode::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GameMode.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GameMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void GameMode::listenError()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GameMode::gameStart()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void GameMode::MsgGameReady(PlayerRole _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GameMode::MsgChat(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GameMode::PlayerJoin()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
