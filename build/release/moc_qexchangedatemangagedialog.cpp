/****************************************************************************
** Meta object code from reading C++ file 'qexchangedatemangagedialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qexchangedatemangagedialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qexchangedatemangagedialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QExchangeDateMangageDialog_t {
    QByteArrayData data[8];
    char stringdata[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QExchangeDateMangageDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QExchangeDateMangageDialog_t qt_meta_stringdata_QExchangeDateMangageDialog = {
    {
QT_MOC_LITERAL(0, 0, 26), // "QExchangeDateMangageDialog"
QT_MOC_LITERAL(1, 27, 17), // "on_DelBtn_clicked"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 17), // "on_AddBtn_clicked"
QT_MOC_LITERAL(4, 64, 36), // "on_calendarWidget_currentPage..."
QT_MOC_LITERAL(5, 101, 4), // "year"
QT_MOC_LITERAL(6, 106, 5), // "month"
QT_MOC_LITERAL(7, 112, 19) // "on_CloseBtn_clicked"

    },
    "QExchangeDateMangageDialog\0on_DelBtn_clicked\0"
    "\0on_AddBtn_clicked\0"
    "on_calendarWidget_currentPageChanged\0"
    "year\0month\0on_CloseBtn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QExchangeDateMangageDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    2,   36,    2, 0x08 /* Private */,
       7,    0,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void,

       0        // eod
};

void QExchangeDateMangageDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QExchangeDateMangageDialog *_t = static_cast<QExchangeDateMangageDialog *>(_o);
        switch (_id) {
        case 0: _t->on_DelBtn_clicked(); break;
        case 1: _t->on_AddBtn_clicked(); break;
        case 2: _t->on_calendarWidget_currentPageChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->on_CloseBtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject QExchangeDateMangageDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QExchangeDateMangageDialog.data,
      qt_meta_data_QExchangeDateMangageDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QExchangeDateMangageDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QExchangeDateMangageDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QExchangeDateMangageDialog.stringdata))
        return static_cast<void*>(const_cast< QExchangeDateMangageDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QExchangeDateMangageDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
