/****************************************************************************
** Meta object code from reading C++ file 'LevelObjectList.h'
**
** Created: Sun 22. May 17:05:47 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "LevelObjectList.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LevelObjectList.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LevelObjectList[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   17,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LevelObjectList[] = {
    "LevelObjectList\0\0item\0"
    "itemDoubleClicked(QListWidgetItem*)\0"
};

const QMetaObject LevelObjectList::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_LevelObjectList,
      qt_meta_data_LevelObjectList, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LevelObjectList::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LevelObjectList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LevelObjectList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LevelObjectList))
        return static_cast<void*>(const_cast< LevelObjectList*>(this));
    return QDialog::qt_metacast(_clname);
}

int LevelObjectList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
