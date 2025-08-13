/****************************************************************************
** Meta object code from reading C++ file 'PlayerControlsWidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/ui/PlayerControlsWidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PlayerControlsWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN20PlayerControlsWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto PlayerControlsWidget::qt_create_metaobjectdata<qt_meta_tag_ZN20PlayerControlsWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PlayerControlsWidget",
        "playPauseClicked",
        "",
        "stopClicked",
        "nextClicked",
        "prevClicked",
        "shuffleToggled",
        "checked",
        "repeatClicked",
        "volumeChanged",
        "value",
        "seeked",
        "position",
        "onPlaybackStateChanged",
        "sf::SoundSource::Status",
        "status",
        "onProgressUpdated",
        "currentSeconds",
        "totalSeconds",
        "onVolumeChanged",
        "volume",
        "setRepeatButtonMode",
        "RepeatMode",
        "mode"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'playPauseClicked'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'stopClicked'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'nextClicked'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'prevClicked'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'shuffleToggled'
        QtMocHelpers::SignalData<void(bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 7 },
        }}),
        // Signal 'repeatClicked'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'volumeChanged'
        QtMocHelpers::SignalData<void(int)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 10 },
        }}),
        // Signal 'seeked'
        QtMocHelpers::SignalData<void(int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 },
        }}),
        // Slot 'onPlaybackStateChanged'
        QtMocHelpers::SlotData<void(sf::SoundSource::Status)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 14, 15 },
        }}),
        // Slot 'onProgressUpdated'
        QtMocHelpers::SlotData<void(int, int)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 }, { QMetaType::Int, 18 },
        }}),
        // Slot 'onVolumeChanged'
        QtMocHelpers::SlotData<void(float)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 20 },
        }}),
        // Slot 'setRepeatButtonMode'
        QtMocHelpers::SlotData<void(RepeatMode)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 22, 23 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PlayerControlsWidget, qt_meta_tag_ZN20PlayerControlsWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PlayerControlsWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20PlayerControlsWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20PlayerControlsWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20PlayerControlsWidgetE_t>.metaTypes,
    nullptr
} };

void PlayerControlsWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PlayerControlsWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->playPauseClicked(); break;
        case 1: _t->stopClicked(); break;
        case 2: _t->nextClicked(); break;
        case 3: _t->prevClicked(); break;
        case 4: _t->shuffleToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->repeatClicked(); break;
        case 6: _t->volumeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->seeked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->onPlaybackStateChanged((*reinterpret_cast< std::add_pointer_t<sf::SoundSource::Status>>(_a[1]))); break;
        case 9: _t->onProgressUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 10: _t->onVolumeChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 11: _t->setRepeatButtonMode((*reinterpret_cast< std::add_pointer_t<RepeatMode>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PlayerControlsWidget::*)()>(_a, &PlayerControlsWidget::playPauseClicked, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlayerControlsWidget::*)()>(_a, &PlayerControlsWidget::stopClicked, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlayerControlsWidget::*)()>(_a, &PlayerControlsWidget::nextClicked, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlayerControlsWidget::*)()>(_a, &PlayerControlsWidget::prevClicked, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlayerControlsWidget::*)(bool )>(_a, &PlayerControlsWidget::shuffleToggled, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlayerControlsWidget::*)()>(_a, &PlayerControlsWidget::repeatClicked, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlayerControlsWidget::*)(int )>(_a, &PlayerControlsWidget::volumeChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlayerControlsWidget::*)(int )>(_a, &PlayerControlsWidget::seeked, 7))
            return;
    }
}

const QMetaObject *PlayerControlsWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlayerControlsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20PlayerControlsWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PlayerControlsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void PlayerControlsWidget::playPauseClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void PlayerControlsWidget::stopClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void PlayerControlsWidget::nextClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PlayerControlsWidget::prevClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void PlayerControlsWidget::shuffleToggled(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void PlayerControlsWidget::repeatClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void PlayerControlsWidget::volumeChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void PlayerControlsWidget::seeked(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}
QT_WARNING_POP
