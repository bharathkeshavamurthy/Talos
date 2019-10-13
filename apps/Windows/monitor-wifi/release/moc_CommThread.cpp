/****************************************************************************
** Meta object code from reading C++ file 'CommThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../monitor_wifi/CommThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CommThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CommThread_t {
    QByteArrayData data[50];
    char stringdata0[474];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommThread_t qt_meta_stringdata_CommThread = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CommThread"
QT_MOC_LITERAL(1, 11, 16), // "connectionClosed"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 8), // "newImage"
QT_MOC_LITERAL(4, 38, 13), // "updateUiState"
QT_MOC_LITERAL(5, 52, 7), // "uint8_t"
QT_MOC_LITERAL(6, 60, 9), // "updateFps"
QT_MOC_LITERAL(7, 70, 13), // "newBinaryData"
QT_MOC_LITERAL(8, 84, 12), // "newAsciiData"
QT_MOC_LITERAL(9, 97, 14), // "cannotOpenPort"
QT_MOC_LITERAL(10, 112, 1), // "s"
QT_MOC_LITERAL(11, 114, 11), // "showVersion"
QT_MOC_LITERAL(12, 126, 9), // "reconnect"
QT_MOC_LITERAL(13, 136, 9), // "connected"
QT_MOC_LITERAL(14, 146, 12), // "disconnected"
QT_MOC_LITERAL(15, 159, 12), // "bytesWritten"
QT_MOC_LITERAL(16, 172, 5), // "bytes"
QT_MOC_LITERAL(17, 178, 9), // "readyRead"
QT_MOC_LITERAL(18, 188, 10), // "timerEvent"
QT_MOC_LITERAL(19, 199, 9), // "goForward"
QT_MOC_LITERAL(20, 209, 10), // "goBackward"
QT_MOC_LITERAL(21, 220, 6), // "goLeft"
QT_MOC_LITERAL(22, 227, 7), // "goRight"
QT_MOC_LITERAL(23, 235, 10), // "stopMotors"
QT_MOC_LITERAL(24, 246, 8), // "led0Slot"
QT_MOC_LITERAL(25, 255, 5), // "state"
QT_MOC_LITERAL(26, 261, 8), // "led1Slot"
QT_MOC_LITERAL(27, 270, 8), // "led2Slot"
QT_MOC_LITERAL(28, 279, 8), // "led3Slot"
QT_MOC_LITERAL(29, 288, 8), // "led4Slot"
QT_MOC_LITERAL(30, 297, 8), // "led5Slot"
QT_MOC_LITERAL(31, 306, 8), // "led6Slot"
QT_MOC_LITERAL(32, 315, 8), // "led7Slot"
QT_MOC_LITERAL(33, 324, 8), // "led8Slot"
QT_MOC_LITERAL(34, 333, 8), // "led9Slot"
QT_MOC_LITERAL(35, 342, 10), // "sound1Slot"
QT_MOC_LITERAL(36, 353, 10), // "sound2Slot"
QT_MOC_LITERAL(37, 364, 10), // "sound3Slot"
QT_MOC_LITERAL(38, 375, 10), // "sound4Slot"
QT_MOC_LITERAL(39, 386, 10), // "sound5Slot"
QT_MOC_LITERAL(40, 397, 12), // "audioOffSlot"
QT_MOC_LITERAL(41, 410, 9), // "updateRed"
QT_MOC_LITERAL(42, 420, 5), // "value"
QT_MOC_LITERAL(43, 426, 11), // "updateGreen"
QT_MOC_LITERAL(44, 438, 10), // "updateBlue"
QT_MOC_LITERAL(45, 449, 16), // "updateParameters"
QT_MOC_LITERAL(46, 466, 1), // "t"
QT_MOC_LITERAL(47, 468, 1), // "w"
QT_MOC_LITERAL(48, 470, 1), // "h"
QT_MOC_LITERAL(49, 472, 1) // "z"

    },
    "CommThread\0connectionClosed\0\0newImage\0"
    "updateUiState\0uint8_t\0updateFps\0"
    "newBinaryData\0newAsciiData\0cannotOpenPort\0"
    "s\0showVersion\0reconnect\0connected\0"
    "disconnected\0bytesWritten\0bytes\0"
    "readyRead\0timerEvent\0goForward\0"
    "goBackward\0goLeft\0goRight\0stopMotors\0"
    "led0Slot\0state\0led1Slot\0led2Slot\0"
    "led3Slot\0led4Slot\0led5Slot\0led6Slot\0"
    "led7Slot\0led8Slot\0led9Slot\0sound1Slot\0"
    "sound2Slot\0sound3Slot\0sound4Slot\0"
    "sound5Slot\0audioOffSlot\0updateRed\0"
    "value\0updateGreen\0updateBlue\0"
    "updateParameters\0t\0w\0h\0z"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      39,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  209,    2, 0x06 /* Public */,
       3,    0,  210,    2, 0x06 /* Public */,
       4,    1,  211,    2, 0x06 /* Public */,
       6,    0,  214,    2, 0x06 /* Public */,
       7,    0,  215,    2, 0x06 /* Public */,
       8,    0,  216,    2, 0x06 /* Public */,
       9,    1,  217,    2, 0x06 /* Public */,
      11,    2,  220,    2, 0x06 /* Public */,
      12,    0,  225,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,  226,    2, 0x0a /* Public */,
      14,    0,  227,    2, 0x0a /* Public */,
      15,    1,  228,    2, 0x0a /* Public */,
      17,    0,  231,    2, 0x0a /* Public */,
      18,    0,  232,    2, 0x0a /* Public */,
      19,    0,  233,    2, 0x0a /* Public */,
      20,    0,  234,    2, 0x0a /* Public */,
      21,    0,  235,    2, 0x0a /* Public */,
      22,    0,  236,    2, 0x0a /* Public */,
      23,    0,  237,    2, 0x0a /* Public */,
      24,    1,  238,    2, 0x0a /* Public */,
      26,    1,  241,    2, 0x0a /* Public */,
      27,    1,  244,    2, 0x0a /* Public */,
      28,    1,  247,    2, 0x0a /* Public */,
      29,    1,  250,    2, 0x0a /* Public */,
      30,    1,  253,    2, 0x0a /* Public */,
      31,    1,  256,    2, 0x0a /* Public */,
      32,    1,  259,    2, 0x0a /* Public */,
      33,    1,  262,    2, 0x0a /* Public */,
      34,    1,  265,    2, 0x0a /* Public */,
      35,    0,  268,    2, 0x0a /* Public */,
      36,    0,  269,    2, 0x0a /* Public */,
      37,    0,  270,    2, 0x0a /* Public */,
      38,    0,  271,    2, 0x0a /* Public */,
      39,    0,  272,    2, 0x0a /* Public */,
      40,    0,  273,    2, 0x0a /* Public */,
      41,    1,  274,    2, 0x0a /* Public */,
      43,    1,  277,    2, 0x0a /* Public */,
      44,    1,  280,    2, 0x0a /* Public */,
      45,    4,  283,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   46,   47,   48,   49,

       0        // eod
};

void CommThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CommThread *_t = static_cast<CommThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectionClosed(); break;
        case 1: _t->newImage(); break;
        case 2: _t->updateUiState((*reinterpret_cast< uint8_t(*)>(_a[1]))); break;
        case 3: _t->updateFps(); break;
        case 4: _t->newBinaryData(); break;
        case 5: _t->newAsciiData(); break;
        case 6: _t->cannotOpenPort((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->showVersion((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->reconnect(); break;
        case 9: _t->connected(); break;
        case 10: _t->disconnected(); break;
        case 11: _t->bytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 12: _t->readyRead(); break;
        case 13: _t->timerEvent(); break;
        case 14: _t->goForward(); break;
        case 15: _t->goBackward(); break;
        case 16: _t->goLeft(); break;
        case 17: _t->goRight(); break;
        case 18: _t->stopMotors(); break;
        case 19: _t->led0Slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->led1Slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->led2Slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->led3Slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->led4Slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->led5Slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->led6Slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->led7Slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->led8Slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->led9Slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->sound1Slot(); break;
        case 30: _t->sound2Slot(); break;
        case 31: _t->sound3Slot(); break;
        case 32: _t->sound4Slot(); break;
        case 33: _t->sound5Slot(); break;
        case 34: _t->audioOffSlot(); break;
        case 35: _t->updateRed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->updateGreen((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->updateBlue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: _t->updateParameters((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (CommThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommThread::connectionClosed)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CommThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommThread::newImage)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CommThread::*_t)(uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommThread::updateUiState)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CommThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommThread::updateFps)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (CommThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommThread::newBinaryData)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (CommThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommThread::newAsciiData)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (CommThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommThread::cannotOpenPort)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (CommThread::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommThread::showVersion)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (CommThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommThread::reconnect)) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject CommThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CommThread.data,
      qt_meta_data_CommThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CommThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CommThread.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CommThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 39)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 39;
    }
    return _id;
}

// SIGNAL 0
void CommThread::connectionClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CommThread::newImage()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CommThread::updateUiState(uint8_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CommThread::updateFps()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CommThread::newBinaryData()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void CommThread::newAsciiData()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CommThread::cannotOpenPort(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CommThread::showVersion(QString _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CommThread::reconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
