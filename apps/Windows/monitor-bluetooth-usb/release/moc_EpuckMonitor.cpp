/****************************************************************************
** Meta object code from reading C++ file 'EpuckMonitor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../monitor/EpuckMonitor.h"
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
    QByteArrayData data[38];
    char stringdata0[368];
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
QT_MOC_LITERAL(7, 36, 11), // "moveForward"
QT_MOC_LITERAL(8, 48, 10), // "motorSpeed"
QT_MOC_LITERAL(9, 59, 12), // "moveBackward"
QT_MOC_LITERAL(10, 72, 8), // "moveLeft"
QT_MOC_LITERAL(11, 81, 9), // "moveRight"
QT_MOC_LITERAL(12, 91, 14), // "connectToRobot"
QT_MOC_LITERAL(13, 106, 5), // "char*"
QT_MOC_LITERAL(14, 112, 8), // "portName"
QT_MOC_LITERAL(15, 121, 11), // "new_x_angle"
QT_MOC_LITERAL(16, 133, 5), // "value"
QT_MOC_LITERAL(17, 139, 11), // "new_y_angle"
QT_MOC_LITERAL(18, 151, 11), // "new_z_angle"
QT_MOC_LITERAL(19, 163, 7), // "connect"
QT_MOC_LITERAL(20, 171, 10), // "disconnect"
QT_MOC_LITERAL(21, 182, 16), // "updateParameters"
QT_MOC_LITERAL(22, 199, 4), // "goUp"
QT_MOC_LITERAL(23, 204, 6), // "goDown"
QT_MOC_LITERAL(24, 211, 6), // "goLeft"
QT_MOC_LITERAL(25, 218, 7), // "goRight"
QT_MOC_LITERAL(26, 226, 16), // "sensorActivation"
QT_MOC_LITERAL(27, 243, 5), // "state"
QT_MOC_LITERAL(28, 249, 11), // "updateSpeed"
QT_MOC_LITERAL(29, 261, 19), // "binarySensorsUpdate"
QT_MOC_LITERAL(30, 281, 18), // "asciiSensorsUpdate"
QT_MOC_LITERAL(31, 300, 12), // "cameraUpdate"
QT_MOC_LITERAL(32, 313, 9), // "getImages"
QT_MOC_LITERAL(33, 323, 12), // "printMessage"
QT_MOC_LITERAL(34, 336, 1), // "s"
QT_MOC_LITERAL(35, 338, 10), // "portOpened"
QT_MOC_LITERAL(36, 349, 4), // "test"
QT_MOC_LITERAL(37, 354, 13) // "updateRgbLeds"

    },
    "EpuckMonitor\0newParameters\0\0t\0w\0h\0z\0"
    "moveForward\0motorSpeed\0moveBackward\0"
    "moveLeft\0moveRight\0connectToRobot\0"
    "char*\0portName\0new_x_angle\0value\0"
    "new_y_angle\0new_z_angle\0connect\0"
    "disconnect\0updateParameters\0goUp\0"
    "goDown\0goLeft\0goRight\0sensorActivation\0"
    "state\0updateSpeed\0binarySensorsUpdate\0"
    "asciiSensorsUpdate\0cameraUpdate\0"
    "getImages\0printMessage\0s\0portOpened\0"
    "test\0updateRgbLeds"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EpuckMonitor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,  144,    2, 0x06 /* Public */,
       7,    1,  153,    2, 0x06 /* Public */,
       9,    1,  156,    2, 0x06 /* Public */,
      10,    1,  159,    2, 0x06 /* Public */,
      11,    1,  162,    2, 0x06 /* Public */,
      12,    1,  165,    2, 0x06 /* Public */,
      15,    1,  168,    2, 0x06 /* Public */,
      17,    1,  171,    2, 0x06 /* Public */,
      18,    1,  174,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    0,  177,    2, 0x0a /* Public */,
      20,    0,  178,    2, 0x0a /* Public */,
      21,    0,  179,    2, 0x0a /* Public */,
      22,    0,  180,    2, 0x0a /* Public */,
      23,    0,  181,    2, 0x0a /* Public */,
      24,    0,  182,    2, 0x0a /* Public */,
      25,    0,  183,    2, 0x0a /* Public */,
      26,    1,  184,    2, 0x0a /* Public */,
      28,    0,  187,    2, 0x0a /* Public */,
      29,    0,  188,    2, 0x0a /* Public */,
      30,    0,  189,    2, 0x0a /* Public */,
      31,    0,  190,    2, 0x0a /* Public */,
      32,    0,  191,    2, 0x0a /* Public */,
      33,    1,  192,    2, 0x0a /* Public */,
      35,    0,  195,    2, 0x0a /* Public */,
      36,    0,  196,    2, 0x0a /* Public */,
      37,    0,  197,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   16,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   34,
    QMetaType::Void,
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
        case 1: _t->moveForward((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->moveBackward((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->moveLeft((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->moveRight((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->connectToRobot((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 6: _t->new_x_angle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->new_y_angle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->new_z_angle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->connect(); break;
        case 10: _t->disconnect(); break;
        case 11: _t->updateParameters(); break;
        case 12: _t->goUp(); break;
        case 13: _t->goDown(); break;
        case 14: _t->goLeft(); break;
        case 15: _t->goRight(); break;
        case 16: _t->sensorActivation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->updateSpeed(); break;
        case 18: _t->binarySensorsUpdate(); break;
        case 19: _t->asciiSensorsUpdate(); break;
        case 20: _t->cameraUpdate(); break;
        case 21: _t->getImages(); break;
        case 22: _t->printMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->portOpened(); break;
        case 24: _t->test(); break;
        case 25: _t->updateRgbLeds(); break;
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
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EpuckMonitor::moveForward)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (EpuckMonitor::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EpuckMonitor::moveBackward)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (EpuckMonitor::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EpuckMonitor::moveLeft)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (EpuckMonitor::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EpuckMonitor::moveRight)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (EpuckMonitor::*_t)(char * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EpuckMonitor::connectToRobot)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (EpuckMonitor::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EpuckMonitor::new_x_angle)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (EpuckMonitor::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EpuckMonitor::new_y_angle)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (EpuckMonitor::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EpuckMonitor::new_z_angle)) {
                *result = 8;
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
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 26;
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
void EpuckMonitor::moveForward(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EpuckMonitor::moveBackward(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void EpuckMonitor::moveLeft(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void EpuckMonitor::moveRight(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void EpuckMonitor::connectToRobot(char * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void EpuckMonitor::new_x_angle(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void EpuckMonitor::new_y_angle(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void EpuckMonitor::new_z_angle(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
