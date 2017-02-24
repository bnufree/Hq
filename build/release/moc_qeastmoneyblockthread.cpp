/****************************************************************************
** Meta object code from reading C++ file 'qeastmoneyblockthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qeastmoneyblockthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qeastmoneyblockthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QEastMoneyBlockThread_t {
    QByteArrayData data[23];
    char stringdata[318];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QEastMoneyBlockThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QEastMoneyBlockThread_t qt_meta_stringdata_QEastMoneyBlockThread = {
    {
QT_MOC_LITERAL(0, 0, 21), // "QEastMoneyBlockThread"
QT_MOC_LITERAL(1, 22, 17), // "sendBlockDataList"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 13), // "BlockDataList"
QT_MOC_LITERAL(4, 55, 4), // "list"
QT_MOC_LITERAL(5, 60, 24), // "updateBlockCodesFinished"
QT_MOC_LITERAL(6, 85, 25), // "sendStkinfoUpdateProgress"
QT_MOC_LITERAL(7, 111, 3), // "cur"
QT_MOC_LITERAL(8, 115, 5), // "total"
QT_MOC_LITERAL(9, 121, 15), // "signalUpdateMsg"
QT_MOC_LITERAL(10, 137, 3), // "msg"
QT_MOC_LITERAL(11, 141, 10), // "setOptType"
QT_MOC_LITERAL(12, 152, 14), // "BLOCK_OPT_TYPE"
QT_MOC_LITERAL(13, 167, 4), // "type"
QT_MOC_LITERAL(14, 172, 11), // "setSortRule"
QT_MOC_LITERAL(15, 184, 18), // "BLOCK_DISPLAY_RULE"
QT_MOC_LITERAL(16, 203, 4), // "rule"
QT_MOC_LITERAL(17, 208, 15), // "reverseSortRule"
QT_MOC_LITERAL(18, 224, 19), // "SetUpdateBlockCodes"
QT_MOC_LITERAL(19, 244, 7), // "pUpdate"
QT_MOC_LITERAL(20, 252, 22), // "slotWorkThreadFinished"
QT_MOC_LITERAL(21, 275, 23), // "slotStkCodeUpdateFinish"
QT_MOC_LITERAL(22, 299, 18) // "slotUpdateFHSPInfo"

    },
    "QEastMoneyBlockThread\0sendBlockDataList\0"
    "\0BlockDataList\0list\0updateBlockCodesFinished\0"
    "sendStkinfoUpdateProgress\0cur\0total\0"
    "signalUpdateMsg\0msg\0setOptType\0"
    "BLOCK_OPT_TYPE\0type\0setSortRule\0"
    "BLOCK_DISPLAY_RULE\0rule\0reverseSortRule\0"
    "SetUpdateBlockCodes\0pUpdate\0"
    "slotWorkThreadFinished\0slotStkCodeUpdateFinish\0"
    "slotUpdateFHSPInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QEastMoneyBlockThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       5,    0,   77,    2, 0x06 /* Public */,
       6,    2,   78,    2, 0x06 /* Public */,
       9,    1,   83,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   86,    2, 0x0a /* Public */,
      14,    1,   89,    2, 0x0a /* Public */,
      17,    0,   92,    2, 0x0a /* Public */,
      18,    1,   93,    2, 0x0a /* Public */,
      18,    0,   96,    2, 0x2a /* Public | MethodCloned */,
      20,    0,   97,    2, 0x0a /* Public */,
      21,    2,   98,    2, 0x0a /* Public */,
      22,    0,  103,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void, QMetaType::QString,   10,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void,

       0        // eod
};

void QEastMoneyBlockThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QEastMoneyBlockThread *_t = static_cast<QEastMoneyBlockThread *>(_o);
        switch (_id) {
        case 0: _t->sendBlockDataList((*reinterpret_cast< const BlockDataList(*)>(_a[1]))); break;
        case 1: _t->updateBlockCodesFinished(); break;
        case 2: _t->sendStkinfoUpdateProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->signalUpdateMsg((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->setOptType((*reinterpret_cast< BLOCK_OPT_TYPE(*)>(_a[1]))); break;
        case 5: _t->setSortRule((*reinterpret_cast< BLOCK_DISPLAY_RULE(*)>(_a[1]))); break;
        case 6: _t->reverseSortRule(); break;
        case 7: _t->SetUpdateBlockCodes((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->SetUpdateBlockCodes(); break;
        case 9: _t->slotWorkThreadFinished(); break;
        case 10: _t->slotStkCodeUpdateFinish((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->slotUpdateFHSPInfo(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QEastMoneyBlockThread::*_t)(const BlockDataList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QEastMoneyBlockThread::sendBlockDataList)) {
                *result = 0;
            }
        }
        {
            typedef void (QEastMoneyBlockThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QEastMoneyBlockThread::updateBlockCodesFinished)) {
                *result = 1;
            }
        }
        {
            typedef void (QEastMoneyBlockThread::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QEastMoneyBlockThread::sendStkinfoUpdateProgress)) {
                *result = 2;
            }
        }
        {
            typedef void (QEastMoneyBlockThread::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QEastMoneyBlockThread::signalUpdateMsg)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject QEastMoneyBlockThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_QEastMoneyBlockThread.data,
      qt_meta_data_QEastMoneyBlockThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QEastMoneyBlockThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QEastMoneyBlockThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QEastMoneyBlockThread.stringdata))
        return static_cast<void*>(const_cast< QEastMoneyBlockThread*>(this));
    return QThread::qt_metacast(_clname);
}

int QEastMoneyBlockThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void QEastMoneyBlockThread::sendBlockDataList(const BlockDataList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QEastMoneyBlockThread::updateBlockCodesFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void QEastMoneyBlockThread::sendStkinfoUpdateProgress(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QEastMoneyBlockThread::signalUpdateMsg(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
