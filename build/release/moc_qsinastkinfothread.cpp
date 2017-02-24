/****************************************************************************
** Meta object code from reading C++ file 'qsinastkinfothread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qsinastkinfothread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qsinastkinfothread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QSinaStkInfoThread_t {
    QByteArrayData data[15];
    char stringdata[163];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QSinaStkInfoThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QSinaStkInfoThread_t qt_meta_stringdata_QSinaStkInfoThread = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QSinaStkInfoThread"
QT_MOC_LITERAL(1, 19, 15), // "sendStkDataList"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 13), // "StockDataList"
QT_MOC_LITERAL(4, 50, 4), // "list"
QT_MOC_LITERAL(5, 55, 10), // "setOptType"
QT_MOC_LITERAL(6, 66, 16), // "STK_DISPLAY_TYPE"
QT_MOC_LITERAL(7, 83, 4), // "type"
QT_MOC_LITERAL(8, 88, 11), // "setSortRule"
QT_MOC_LITERAL(9, 100, 16), // "STK_DISPLAY_RULE"
QT_MOC_LITERAL(10, 117, 4), // "rule"
QT_MOC_LITERAL(11, 122, 15), // "reverseSortRule"
QT_MOC_LITERAL(12, 138, 10), // "setStkList"
QT_MOC_LITERAL(13, 149, 9), // "setActive"
QT_MOC_LITERAL(14, 159, 3) // "act"

    },
    "QSinaStkInfoThread\0sendStkDataList\0\0"
    "StockDataList\0list\0setOptType\0"
    "STK_DISPLAY_TYPE\0type\0setSortRule\0"
    "STK_DISPLAY_RULE\0rule\0reverseSortRule\0"
    "setStkList\0setActive\0act"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QSinaStkInfoThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   47,    2, 0x0a /* Public */,
       8,    1,   50,    2, 0x0a /* Public */,
      11,    0,   53,    2, 0x0a /* Public */,
      12,    1,   54,    2, 0x0a /* Public */,
      13,    1,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,    4,
    QMetaType::Void, QMetaType::Bool,   14,

       0        // eod
};

void QSinaStkInfoThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QSinaStkInfoThread *_t = static_cast<QSinaStkInfoThread *>(_o);
        switch (_id) {
        case 0: _t->sendStkDataList((*reinterpret_cast< const StockDataList(*)>(_a[1]))); break;
        case 1: _t->setOptType((*reinterpret_cast< STK_DISPLAY_TYPE(*)>(_a[1]))); break;
        case 2: _t->setSortRule((*reinterpret_cast< STK_DISPLAY_RULE(*)>(_a[1]))); break;
        case 3: _t->reverseSortRule(); break;
        case 4: _t->setStkList((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 5: _t->setActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QSinaStkInfoThread::*_t)(const StockDataList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSinaStkInfoThread::sendStkDataList)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject QSinaStkInfoThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_QSinaStkInfoThread.data,
      qt_meta_data_QSinaStkInfoThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QSinaStkInfoThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QSinaStkInfoThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QSinaStkInfoThread.stringdata))
        return static_cast<void*>(const_cast< QSinaStkInfoThread*>(this));
    return QThread::qt_metacast(_clname);
}

int QSinaStkInfoThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void QSinaStkInfoThread::sendStkDataList(const StockDataList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
