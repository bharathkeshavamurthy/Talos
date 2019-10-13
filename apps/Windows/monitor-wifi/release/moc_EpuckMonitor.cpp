/****************************************************************************
** Meta object code from reading C++ file 'EpuckMonitor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../monitor_wifi/EpuckMonitor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EpuckMonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EpuckMonitor_t {
    QByteArrayData data[27];
    char stringdata0[258];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EpuckMonitor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EpuckMonitor_t qt_meta_stringdata_EpuckMonitor = {
    {
QT_MOC_LITERAL(0, 0, 12), // "EpuckMonitor"
QT_MOC_LITERAL(1, 13, 13), // "newParameters"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 1), // "t"
QT_MOC_LITERAL(4, 30, 1), // "w"
QT_MOC_LITERAL(5, 32, 1), // "h"
QT_MOC_LITERAL(6, 34, 1), // "z"
QT_MOC_LITERAL(7, 36, 11), // "new_x_angle"
QT_MOC_LITERAL(8, 48, 5), // "value"
QT_MOC_LITERAL(9, 54, 11), // "new_y_angle"
QT_MOC_LITERAL(10, 66, 11), // "new_z_angle"
QT_MOC_LITERAL(11, 78, 13), // "updateUiState"
QT_MOC_LITERAL(12, 92, 7), // "uint8_t"
QT_MOC_LITERAL(13, 100, 5), // "state"
QT_MOC_LITERAL(14, 106, 9), // "updateFps"
QT_MOC_LITERAL(15, 116, 7), // "connect"
QT_MOC_LITERAL(16, 124, 10), // "disconnect"
QT_MOC_LITERAL(17, 135, 16), // "updateParameters"
QT_MOC_LITERAL(18, 152, 16), // "sensorActivation"
QT_MOC_LITERAL(19, 169, 11), // "updateSpeed"
QT_MOC_LITERAL(20, 181, 19), // "binarySensorsUpdate"
QT_MOC_LITERAL(21, 201, 12), // "cameraUpdate"
QT_MOC_LITERAL(22, 214, 9), // "getImages"
QT_MOC_LITERAL(23, 224, 12), // "printMessage"
QT_MOC_LITERAL(24, 237, 1), // "s"
QT_MOC_LITERAL(25, 239, 4), // "test"
QT_MOC_LITERAL(26, 244, 13) // "updateRgbLeds"

    },
    "EpuckMonitor\0newParameters\0\0t\0w\0h\0z\0"
    "new_x_angle\0value\0new_y_angle\0new_z_angle\0"
    "updateUiState\0uint8_t\0state\0updateFps\0"
    "connect\0disconnect\0updateParameters\0"
    "sensorActivation\0updateSpeed\0"
    "binarySensorsUpdate\0cameraUpdate\0"
    "getImages\0printMessage\0s\0test\0"
    "updateRgbLeds"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EpuckMonitor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   99,    2, 0x06 /* Public */,
       7,    1,  108,    2, 0x06 /* Public */,
       9,    1,  111,    2, 0x06 /* Public */,
      10,    1,  114,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,  117,    2, 0x0a /* Public */,
      14,    0,  120,    2, 0x0a /* Public */,
      15,    0,  121,    2, 0x0a /* Public */,
      16,    0,  122,    2, 0x0a /* Public */,
      17,    0,  123,    2, 0x0a /* Public */,
      18,    1,  124,    2, 0x0a /* Public */,
      19,    0,  127,    2, 0x0a /* Public */,
      20,    0,  128,    2, 0x0a /* Public */,
      21,    0,  129,    2, 0x0a /* Public */,
      22,    0,  130,    2, 0x0a /* Public */,
      23,    1,  131,    2, 0x0a /* Public */,
      25,    0,  134,    2, 0x0a /* Public */,
      26,    0,  135,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void EpuckMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EpuckMonitor *_t = static_cast<EpuckMonitor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newParameters((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: _t->new_x_angle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->new_y_angle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->new_z_angle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->updateUiState((*reinterpret_cast< uint8_t(*)>(_a[1]))); break;
        case 5: _t->updateFps(); break;
        case 6: _t->connect(); break;
        case 7: _t->disconnect(); break;
        case 8: _t->updateParameters(); break;
        case 9: _t->sensorActivation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->updateSpeed(); break;
        case 11: _t->binarySensorsUpdate(); break;
        case 12: _t->cameraUpdate(); break;
        case 13: _t->getImages(); break;
        case 14: _t->printMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->test(); break;
        case 16: _t->updateRgbLeds(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (EpuckMonitor::*_t)(int , int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EpuckMonitor::newParameters)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (EpuckMonitor::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EpuckMonitor::new_x_angle)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (EpuckMonitor::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EpuckMonitor::new_y_angle)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (EpuckMonitor::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EpuckMonitor::new_z_angle)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject EpuckMonitor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_EpuckMonitor.data,
      qt_meta_data_EpuckMonitor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *EpuckMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EpuckMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EpuckMonitor.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int EpuckMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void EpuckMonitor::newParameters(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EpuckMonitor::new_x_angle(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EpuckMonitor::new_y_angle(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void EpuckMonitor::new_z_angle(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
