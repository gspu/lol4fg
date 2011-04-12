/****************************************************************************
** Meta object code from reading C++ file 'ObjectDetailsDialog.h'
**
** Created: Sat 2. Apr 17:46:25 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ObjectDetailsDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ObjectDetailsDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ObjectDetailsDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x0a,
      30,   20,   20,   20, 0x0a,
      38,   20,   20,   20, 0x0a,
      46,   20,   20,   20, 0x0a,
      62,   20,   20,   20, 0x0a,
      77,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ObjectDetailsDialog[] = {
    "ObjectDetailsDialog\0\0accept()\0apply()\0"
    "close()\0showMatSelect()\0showPSSelect()\0"
    "showSoundSelect()\0"
};

const QMetaObject ObjectDetailsDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ObjectDetailsDialog,
      qt_meta_data_ObjectDetailsDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ObjectDetailsDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ObjectDetailsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ObjectDetailsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ObjectDetailsDialog))
        return static_cast<void*>(const_cast< ObjectDetailsDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ObjectDetailsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: accept(); break;
        case 1: apply(); break;
        case 2: close(); break;
        case 3: showMatSelect(); break;
        case 4: showPSSelect(); break;
        case 5: showSoundSelect(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
