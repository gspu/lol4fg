/****************************************************************************
** Meta object code from reading C++ file 'GenericSelectDialog.h'
**
** Created: Tue 4. Jan 23:41:32 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "GenericSelectDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GenericSelectDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GenericSelectDialog[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   21,   20,   20, 0x0a,
      60,   20,   20,   20, 0x0a,
      69,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GenericSelectDialog[] = {
    "GenericSelectDialog\0\0item\0"
    "acceptSelection(QListWidgetItem*)\0"
    "accept()\0reject()\0"
};

const QMetaObject GenericSelectDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_GenericSelectDialog,
      qt_meta_data_GenericSelectDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GenericSelectDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GenericSelectDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GenericSelectDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GenericSelectDialog))
        return static_cast<void*>(const_cast< GenericSelectDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int GenericSelectDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: acceptSelection((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: accept(); break;
        case 2: reject(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
