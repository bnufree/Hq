/****************************************************************************
** Meta object code from reading C++ file 'qeastmoneystockinfothread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qeastmoneystockinfothread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qeastmoneystockinfothread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_qeastmoneystockinfothread_t {
    QByteArrayData data[9];
    char stringdata[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_qeastmoneystockinfothread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_qeastmoneystockinfothread_t qt_meta_stringdata_qeastmoneystockinfothread = {
    {
QT_MOC_LITERAL(0, 0, 25), // "qeastmoneystockinfothread"
QT_MOC_LITERAL(1, 26, 14), // "sendMktCapInfo"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 4), // "code"
QT_MOC_LITERAL(4, 47, 6), // "totala"
QT_MOC_LITERAL(5, 54, 3), // "lta"
QT_MOC_LITERAL(6, 58, 18), // "sendUpdateProgress"
QT_MOC_LITERAL(7, 77, 3), // "cur"
QT_MOC_LITERAL(8, 81, 5) // "total"

    },
    "qeastmoneystockinfothread\0sendMktCapInfo\0"
    "\0code\0totala\0lta\0sendUpdateProgress\0"
    "cur\0total"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_qeastmoneystockinfothread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,
       6,    2,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::LongLong, QMetaType::LongLong,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,

       0        // eod
};

void qeastmoneystockinfothread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        qeastmoneystockinfothread *_t = static_cast<qeastmoneystockinfothread *>(_o);
        switch (_id) {
        case 0: _t->sendMktCapInfo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])),(*reinterpret_cast< qint64(*)>(_a[3]))); break;
        case 1: _t->sendUpdateProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (qeastmoneystockinfothread::*_t)(const QString & , qint64 , qint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&qeastmoneystockinfothread::sendMktCapInfo)) {
                *result = 0;
            }
        }
        {
            typedef void (qeastmoneystockinfothread::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&qeastmoneystockinfothread::sendUpdateProgress)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject qeastmoneystockinfothread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_qeastmoneystockinfothread.data,
      qt_meta_data_qeastmoneystockinfothread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *qeastmoneystockinfothread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *qeastmoneystockinfothread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_qeastmoneystockinfothread.stringdata))
        return static_cast<void*>(const_cast< qeastmoneystockinfothread*>(this));
    return QThread::qt_metacast(_clname);
}

int qeastmoneystockinfothread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void qeastmoneystockinfothread::sendMktCapInfo(const QString & _t1, qint64 _t2, qint64 _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void qeastmoneystockinfothread::sendUpdateProgress(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
