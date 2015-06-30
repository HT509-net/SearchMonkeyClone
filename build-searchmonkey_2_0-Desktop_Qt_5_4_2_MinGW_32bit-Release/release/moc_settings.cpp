/****************************************************************************
** Meta object code from reading C++ file 'settings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SearchMonkeySRC/settings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Settings_t {
    QByteArrayData data[18];
    char stringdata[406];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Settings_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Settings_t qt_meta_stringdata_Settings = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Settings"
QT_MOC_LITERAL(1, 9, 24), // "on_addPushButton_clicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 20), // "on_buttonBox_clicked"
QT_MOC_LITERAL(4, 56, 16), // "QAbstractButton*"
QT_MOC_LITERAL(5, 73, 6), // "button"
QT_MOC_LITERAL(6, 80, 28), // "on_defaultPushButton_clicked"
QT_MOC_LITERAL(7, 109, 27), // "on_deletePushButton_clicked"
QT_MOC_LITERAL(8, 137, 31), // "on_directoriesButtonBox_clicked"
QT_MOC_LITERAL(9, 169, 27), // "on_generalButtonBox_clicked"
QT_MOC_LITERAL(10, 197, 28), // "on_leftAllPushButton_clicked"
QT_MOC_LITERAL(11, 226, 25), // "on_leftPushButton_clicked"
QT_MOC_LITERAL(12, 252, 26), // "on_moreLineEdit_textEdited"
QT_MOC_LITERAL(13, 279, 29), // "on_rightAllPushButton_clicked"
QT_MOC_LITERAL(14, 309, 26), // "on_rightPushButton_clicked"
QT_MOC_LITERAL(15, 336, 32), // "on_searchAllDirsCheckBox_toggled"
QT_MOC_LITERAL(16, 369, 7), // "checked"
QT_MOC_LITERAL(17, 377, 28) // "on_setTypePushButton_clicked"

    },
    "Settings\0on_addPushButton_clicked\0\0"
    "on_buttonBox_clicked\0QAbstractButton*\0"
    "button\0on_defaultPushButton_clicked\0"
    "on_deletePushButton_clicked\0"
    "on_directoriesButtonBox_clicked\0"
    "on_generalButtonBox_clicked\0"
    "on_leftAllPushButton_clicked\0"
    "on_leftPushButton_clicked\0"
    "on_moreLineEdit_textEdited\0"
    "on_rightAllPushButton_clicked\0"
    "on_rightPushButton_clicked\0"
    "on_searchAllDirsCheckBox_toggled\0"
    "checked\0on_setTypePushButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Settings[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    1,   80,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    1,   85,    2, 0x08 /* Private */,
       9,    1,   88,    2, 0x08 /* Private */,
      10,    0,   91,    2, 0x08 /* Private */,
      11,    0,   92,    2, 0x08 /* Private */,
      12,    1,   93,    2, 0x08 /* Private */,
      13,    0,   96,    2, 0x08 /* Private */,
      14,    0,   97,    2, 0x08 /* Private */,
      15,    1,   98,    2, 0x08 /* Private */,
      17,    0,  101,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void,

       0        // eod
};

void Settings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Settings *_t = static_cast<Settings *>(_o);
        switch (_id) {
        case 0: _t->on_addPushButton_clicked(); break;
        case 1: _t->on_buttonBox_clicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 2: _t->on_defaultPushButton_clicked(); break;
        case 3: _t->on_deletePushButton_clicked(); break;
        case 4: _t->on_directoriesButtonBox_clicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 5: _t->on_generalButtonBox_clicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 6: _t->on_leftAllPushButton_clicked(); break;
        case 7: _t->on_leftPushButton_clicked(); break;
        case 8: _t->on_moreLineEdit_textEdited((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->on_rightAllPushButton_clicked(); break;
        case 10: _t->on_rightPushButton_clicked(); break;
        case 11: _t->on_searchAllDirsCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->on_setTypePushButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject Settings::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Settings.data,
      qt_meta_data_Settings,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Settings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Settings::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Settings.stringdata))
        return static_cast<void*>(const_cast< Settings*>(this));
    return QDialog::qt_metacast(_clname);
}

int Settings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
