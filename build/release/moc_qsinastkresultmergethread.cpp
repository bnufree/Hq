/****************************************************************************
** Meta object code from reading C++ file 'qsinastkresultmergethread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qsinastkresultmergethread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qsinastkresultmergethread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QSinaStkResultMergeThread_t {
    QByteArrayData data[31];
    char stringdata[373];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QSinaStkResultMergeThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QSinaStkResultMergeThread_t qt_meta_stringdata_QSinaStkResultMergeThread = {
    {
QT_MOC_LITERAL(0, 0, 25), // "QSinaStkResultMergeThread"
QT_MOC_LITERAL(1, 26, 15), // "sendStkDataList"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 13), // "StockDataList"
QT_MOC_LITERAL(4, 57, 4), // "list"
QT_MOC_LITERAL(5, 62, 25), // "sendStkinfoUpdateProgress"
QT_MOC_LITERAL(6, 88, 3), // "cur"
QT_MOC_LITERAL(7, 92, 5), // "total"
QT_MOC_LITERAL(8, 98, 10), // "setStkList"
QT_MOC_LITERAL(9, 109, 10), // "stopUpdate"
QT_MOC_LITERAL(10, 120, 10), // "setMktType"
QT_MOC_LITERAL(11, 131, 8), // "MKT_TYPE"
QT_MOC_LITERAL(12, 140, 4), // "type"
QT_MOC_LITERAL(13, 145, 11), // "setSortType"
QT_MOC_LITERAL(14, 157, 9), // "setActive"
QT_MOC_LITERAL(15, 167, 6), // "active"
QT_MOC_LITERAL(16, 174, 16), // "setSelfCodesList"
QT_MOC_LITERAL(17, 191, 14), // "updateStkCodes"
QT_MOC_LITERAL(18, 206, 17), // "updateStkInfoList"
QT_MOC_LITERAL(19, 224, 18), // "QList<QStringList>"
QT_MOC_LITERAL(20, 243, 15), // "pStkSectionList"
QT_MOC_LITERAL(21, 259, 12), // "RealtimeInfo"
QT_MOC_LITERAL(22, 272, 5), // "codes"
QT_MOC_LITERAL(23, 278, 14), // "SortResultList"
QT_MOC_LITERAL(24, 293, 14), // "StockDataList&"
QT_MOC_LITERAL(25, 308, 6), // "result"
QT_MOC_LITERAL(26, 315, 3), // "mid"
QT_MOC_LITERAL(27, 319, 14), // "slotRevResList"
QT_MOC_LITERAL(28, 334, 15), // "slotRevZjlxData"
QT_MOC_LITERAL(29, 350, 15), // "QList<zjlxData>"
QT_MOC_LITERAL(30, 366, 6) // "zjlist"

    },
    "QSinaStkResultMergeThread\0sendStkDataList\0"
    "\0StockDataList\0list\0sendStkinfoUpdateProgress\0"
    "cur\0total\0setStkList\0stopUpdate\0"
    "setMktType\0MKT_TYPE\0type\0setSortType\0"
    "setActive\0active\0setSelfCodesList\0"
    "updateStkCodes\0updateStkInfoList\0"
    "QList<QStringList>\0pStkSectionList\0"
    "RealtimeInfo\0codes\0SortResultList\0"
    "StockDataList&\0result\0mid\0slotRevResList\0"
    "slotRevZjlxData\0QList<zjlxData>\0zjlist"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QSinaStkResultMergeThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       5,    2,   87,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   92,    2, 0x0a /* Public */,
       9,    0,   95,    2, 0x0a /* Public */,
      10,    1,   96,    2, 0x0a /* Public */,
      13,    1,   99,    2, 0x0a /* Public */,
      14,    1,  102,    2, 0x0a /* Public */,
      16,    1,  105,    2, 0x0a /* Public */,
      17,    1,  108,    2, 0x08 /* Private */,
      18,    1,  111,    2, 0x08 /* Private */,
      21,    1,  114,    2, 0x08 /* Private */,
      23,    2,  117,    2, 0x08 /* Private */,
      27,    1,  122,    2, 0x08 /* Private */,
      28,    1,  125,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::QStringList,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::QStringList,    4,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 19,   20,
    0x80000000 | 3, QMetaType::QStringList,   22,
    QMetaType::Void, 0x80000000 | 24, 0x80000000 | 3,   25,   26,
    QMetaType::Void, 0x80000000 | 3,   26,
    QMetaType::Void, 0x80000000 | 29,   30,

       0        // eod
};

void QSinaStkResultMergeThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QSinaStkResultMergeThread *_t = static_cast<QSinaStkResultMergeThread *>(_o);
        switch (_id) {
        case 0: _t->sendStkDataList((*reinterpret_cast< const StockDataList(*)>(_a[1]))); break;
        case 1: _t->sendStkinfoUpdateProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->setStkList((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 3: _t->stopUpdate(); break;
        case 4: _t->setMktType((*reinterpret_cast< MKT_TYPE(*)>(_a[1]))); break;
        case 5: _t->setSortType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setSelfCodesList((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 8: _t->updateStkCodes((*reinterpret_cast< MKT_TYPE(*)>(_a[1]))); break;
        case 9: _t->updateStkInfoList((*reinterpret_cast< const QList<QStringList>(*)>(_a[1]))); break;
        case 10: { StockDataList _r = _t->RealtimeInfo((*reinterpret_cast< const QStringList(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< StockDataList*>(_a[0]) = _r; }  break;
        case 11: _t->SortResultList((*reinterpret_cast< StockDataList(*)>(_a[1])),(*reinterpret_cast< const StockDataList(*)>(_a[2]))); break;
        case 12: _t->slotRevResList((*reinterpret_cast< const StockDataList(*)>(_a[1]))); break;
        case 13: _t->slotRevZjlxData((*reinterpret_cast< const QList<zjlxData>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QStringList> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QSinaStkResultMergeThread::*_t)(const StockDataList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSinaStkResultMergeThread::sendStkDataList)) {
                *result = 0;
            }
        }
        {
            typedef void (QSinaStkResultMergeThread::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSinaStkResultMergeThread::sendStkinfoUpdateProgress)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject QSinaStkResultMergeThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_QSinaStkResultMergeThread.data,
      qt_meta_data_QSinaStkResultMergeThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QSinaStkResultMergeThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QSinaStkResultMergeThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QSinaStkResultMergeThread.stringdata))
        return static_cast<void*>(const_cast< QSinaStkResultMergeThread*>(this));
    return QThread::qt_metacast(_clname);
}

int QSinaStkResultMergeThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void QSinaStkResultMergeThread::sendStkDataList(const StockDataList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QSinaStkResultMergeThread::sendStkinfoUpdateProgress(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
