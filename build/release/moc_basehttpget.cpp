/****************************************************************************
** Meta object code from reading C++ file 'basehttpget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../basehttpget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'basehttpget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BaseHttpGet_t {
    QByteArrayData data[8];
    char stringdata[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseHttpGet_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseHttpGet_t qt_meta_stringdata_BaseHttpGet = {
    {
QT_MOC_LITERAL(0, 0, 11), // "BaseHttpGet"
QT_MOC_LITERAL(1, 12, 14), // "signalGetFaild"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 11), // "slotTimeOut"
QT_MOC_LITERAL(4, 40, 17), // "slotFinishedReply"
QT_MOC_LITERAL(5, 58, 14), // "QNetworkReply*"
QT_MOC_LITERAL(6, 73, 5), // "reply"
QT_MOC_LITERAL(7, 79, 20) // "slotReceiveGetFailed"

    },
    "BaseHttpGet\0signalGetFaild\0\0slotTimeOut\0"
    "slotFinishedReply\0QNetworkReply*\0reply\0"
    "slotReceiveGetFailed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseHttpGet[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   35,    2, 0x0a /* Public */,
       4,    1,   36,    2, 0x0a /* Public */,
       7,    0,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,

       0        // eod
};

void BaseHttpGet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BaseHttpGet *_t = static_cast<BaseHttpGet *>(_o);
        switch (_id) {
        case 0: _t->signalGetFaild(); break;
        case 1: _t->slotTimeOut(); break;
        case 2: _t->slotFinishedReply((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 3: _t->slotReceiveGetFailed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BaseHttpGet::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseHttpGet::signalGetFaild)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject BaseHttpGet::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BaseHttpGet.data,
      qt_meta_data_BaseHttpGet,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BaseHttpGet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseHttpGet::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BaseHttpGet.stringdata))
        return static_cast<void*>(const_cast< BaseHttpGet*>(this));
    return QObject::qt_metacast(_clname);
}

int BaseHttpGet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void BaseHttpGet::signalGetFaild()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
