/****************************************************************************
** Meta object code from reading C++ file 'sensorContainer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/IntelliFireUI/include/sensorContainer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sensorContainer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SensorContainer_t {
    QByteArrayData data[8];
    char stringdata0[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SensorContainer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SensorContainer_t qt_meta_stringdata_SensorContainer = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SensorContainer"
QT_MOC_LITERAL(1, 16, 18), // "updateUltrasonicUI"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 5), // "value"
QT_MOC_LITERAL(4, 42, 10), // "updateIRUI"
QT_MOC_LITERAL(5, 53, 7), // "message"
QT_MOC_LITERAL(6, 61, 18), // "sensorValueUpdated"
QT_MOC_LITERAL(7, 80, 2) // "id"

    },
    "SensorContainer\0updateUltrasonicUI\0\0"
    "value\0updateIRUI\0message\0sensorValueUpdated\0"
    "id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SensorContainer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x0a /* Public */,
       4,    1,   32,    2, 0x0a /* Public */,
       6,    2,   35,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Float,    7,    3,

       0        // eod
};

void SensorContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SensorContainer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateUltrasonicUI((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->updateIRUI((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->sensorValueUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SensorContainer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SensorContainer.data,
    qt_meta_data_SensorContainer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SensorContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SensorContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SensorContainer.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IRSensor::IRSensorCallbackInterface"))
        return static_cast< IRSensor::IRSensorCallbackInterface*>(this);
    return QWidget::qt_metacast(_clname);
}

int SensorContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
