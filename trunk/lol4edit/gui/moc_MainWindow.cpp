/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Sat 22. Jan 21:52:30 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EditorMainWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      29,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x0a,
      55,   47,   17,   17, 0x0a,
      70,   17,   17,   17, 0x2a,
      81,   47,   17,   17, 0x0a,
     101,   17,   17,   17, 0x2a,
     117,   47,   17,   17, 0x0a,
     133,   17,   17,   17, 0x2a,
     145,   47,   17,   17, 0x0a,
     162,   17,   17,   17, 0x2a,
     175,   47,   17,   17, 0x0a,
     193,   17,   17,   17, 0x2a,
     207,   47,   17,   17, 0x0a,
     226,   17,   17,   17, 0x2a,
     241,   47,   17,   17, 0x0a,
     260,   17,   17,   17, 0x2a,
     275,   47,   17,   17, 0x0a,
     293,   17,   17,   17, 0x2a,
     307,   47,   17,   17, 0x0a,
     327,   17,   17,   17, 0x2a,
     343,   47,   17,   17, 0x0a,
     362,   17,   17,   17, 0x2a,
     377,   47,   17,   17, 0x0a,
     401,   17,   17,   17, 0x2a,
     421,   47,   17,   17, 0x0a,
     445,   17,   17,   17, 0x2a,
     465,   47,   17,   17, 0x0a,
     489,   17,   17,   17, 0x2a,
     531,  509,   17,   17, 0x0a,
     562,  557,   17,   17, 0x2a,

       0        // eod
};

static const char qt_meta_stringdata_EditorMainWindow[] = {
    "EditorMainWindow\0\0dummy\0performDebugTest(bool)\0"
    "checked\0newLevel(bool)\0newLevel()\0"
    "showOpenLevel(bool)\0showOpenLevel()\0"
    "saveLevel(bool)\0saveLevel()\0"
    "showSaveAs(bool)\0showSaveAs()\0"
    "setGridSnap(bool)\0setGridSnap()\0"
    "setAngleSnap(bool)\0setAngleSnap()\0"
    "setScaleSnap(bool)\0setScaleSnap()\0"
    "setMoveMode(bool)\0setMoveMode()\0"
    "setRotateMode(bool)\0setRotateMode()\0"
    "setScaleMode(bool)\0setScaleMode()\0"
    "setPhysicsEnabled(bool)\0setPhysicsEnabled()\0"
    "setTerrainEditing(bool)\0setTerrainEditing()\0"
    "setObjectsEditing(bool)\0setObjectsEditing()\0"
    "name,confirmOverwrite\0saveAs(Ogre::String,bool)\0"
    "name\0saveAs(Ogre::String)\0"
};

const QMetaObject EditorMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_EditorMainWindow,
      qt_meta_data_EditorMainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EditorMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EditorMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EditorMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EditorMainWindow))
        return static_cast<void*>(const_cast< EditorMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int EditorMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: performDebugTest((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: newLevel((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: newLevel(); break;
        case 3: showOpenLevel((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: showOpenLevel(); break;
        case 5: saveLevel((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: saveLevel(); break;
        case 7: showSaveAs((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: showSaveAs(); break;
        case 9: setGridSnap((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: setGridSnap(); break;
        case 11: setAngleSnap((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: setAngleSnap(); break;
        case 13: setScaleSnap((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: setScaleSnap(); break;
        case 15: setMoveMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: setMoveMode(); break;
        case 17: setRotateMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: setRotateMode(); break;
        case 19: setScaleMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: setScaleMode(); break;
        case 21: setPhysicsEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: setPhysicsEnabled(); break;
        case 23: setTerrainEditing((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: setTerrainEditing(); break;
        case 25: setObjectsEditing((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: setObjectsEditing(); break;
        case 27: saveAs((*reinterpret_cast< Ogre::String(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 28: saveAs((*reinterpret_cast< Ogre::String(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 29;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
