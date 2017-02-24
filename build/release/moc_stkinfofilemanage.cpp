/****************************************************************************
** Meta object code from reading C++ file 'stkinfofilemanage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../stkinfofilemanage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stkinfofilemanage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_StkInfoFileManage_t {
    QByteArrayData data[13];
    char stringdata[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_StkInfoFileManage_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_StkInfoFileManage_t qt_meta_stringdata_StkInfoFileManage = {
    {
QT_MOC_LITERAL(0, 0, 17), // "StkInfoFileManage"
QT_MOC_LITERAL(1, 18, 8), // "setValue"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 6), // "prefix"
QT_MOC_LITERAL(4, 35, 3), // "key"
QT_MOC_LITERAL(5, 39, 5), // "value"
QT_MOC_LITERAL(6, 45, 10), // "setDefault"
QT_MOC_LITERAL(7, 56, 4), // "keys"
QT_MOC_LITERAL(8, 61, 12), // "defaultValue"
QT_MOC_LITERAL(9, 74, 7), // "subkeys"
QT_MOC_LITERAL(10, 82, 10), // "removeKeys"
QT_MOC_LITERAL(11, 93, 11), // "removeGroup"
QT_MOC_LITERAL(12, 105, 5) // "clear"

    },
    "StkInfoFileManage\0setValue\0\0prefix\0"
    "key\0value\0setDefault\0keys\0defaultValue\0"
    "subkeys\0removeKeys\0removeGroup\0clear"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StkInfoFileManage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   54,    2, 0x0a /* Public */,
       6,    3,   61,    2, 0x0a /* Public */,
       5,    3,   68,    2, 0x0a /* Public */,
       5,    2,   75,    2, 0x2a /* Public | MethodCloned */,
       9,    1,   80,    2, 0x0a /* Public */,
      10,    2,   83,    2, 0x0a /* Public */,
      11,    1,   88,    2, 0x0a /* Public */,
      12,    0,   91,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QVariant,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QVariant,    3,    4,    5,
    QMetaType::QVariant, QMetaType::QString, QMetaType::QString, QMetaType::QVariant,    3,    7,    8,
    QMetaType::QVariant, QMetaType::QString, QMetaType::QString,    3,    7,
    QMetaType::QStringList, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QStringList,    3,    7,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void StkInfoFileManage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        StkInfoFileManage *_t = static_cast<StkInfoFileManage *>(_o);
        switch (_id) {
        case 0: _t->setValue((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 1: _t->setDefault((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 2: { QVariant _r = _t->value((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 3: { QVariant _r = _t->value((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 4: { QStringList _r = _t->subkeys((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        case 5: _t->removeKeys((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 6: _t->removeGroup((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->clear(); break;
        default: ;
        }
    }
}

const QMetaObject StkInfoFileManage::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_StkInfoFileManage.data,
      qt_meta_data_StkInfoFileManage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *StkInfoFileManage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StkInfoFileManage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_StkInfoFileManage.stringdata))
        return static_cast<void*>(const_cast< StkInfoFileManage*>(this));
    return QObject::qt_metacast(_clname);
}

int StkInfoFileManage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
