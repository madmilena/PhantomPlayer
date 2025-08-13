/****************************************************************************
** Meta object code from reading C++ file 'PlaybackService.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/services/PlaybackService.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PlaybackService.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN15PlaybackServiceE_t {};
} // unnamed namespace

template <> constexpr inline auto PlaybackService::qt_create_metaobjectdata<qt_meta_tag_ZN15PlaybackServiceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PlaybackService",
        "trackChanged",
        "",
        "Track",
        "track",
        "index",
        "playbackStateChanged",
        "sf::SoundSource::Status",
        "status",
        "progressUpdated",
        "currentSeconds",
        "totalSeconds",
        "volumeChanged",
        "volume",
        "playTrack",
        "togglePlayPause",
        "stop",
        "next",
        "prev",
        "setShuffle",
        "enabled",
        "setRepeatMode",
        "RepeatMode",
        "mode",
        "seek",
        "position",
        "setVolume",
        "onEngineStatusChange"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'trackChanged'
        QtMocHelpers::SignalData<void(const Track &, int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Int, 5 },
        }}),
        // Signal 'playbackStateChanged'
        QtMocHelpers::SignalData<void(sf::SoundSource::Status)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Signal 'progressUpdated'
        QtMocHelpers::SignalData<void(int, int)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 10 }, { QMetaType::Int, 11 },
        }}),
        // Signal 'volumeChanged'
        QtMocHelpers::SignalData<void(float)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 13 },
        }}),
        // Slot 'playTrack'
        QtMocHelpers::SlotData<void(int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Slot 'togglePlayPause'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'stop'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'next'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'prev'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setShuffle'
        QtMocHelpers::SlotData<void(bool)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 20 },
        }}),
        // Slot 'setRepeatMode'
        QtMocHelpers::SlotData<void(RepeatMode)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 22, 23 },
        }}),
        // Slot 'seek'
        QtMocHelpers::SlotData<void(int)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 25 },
        }}),
        // Slot 'setVolume'
        QtMocHelpers::SlotData<void(float)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 13 },
        }}),
        // Slot 'onEngineStatusChange'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PlaybackService, qt_meta_tag_ZN15PlaybackServiceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PlaybackService::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15PlaybackServiceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15PlaybackServiceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15PlaybackServiceE_t>.metaTypes,
    nullptr
} };

void PlaybackService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PlaybackService *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->trackChanged((*reinterpret_cast< std::add_pointer_t<Track>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->playbackStateChanged((*reinterpret_cast< std::add_pointer_t<sf::SoundSource::Status>>(_a[1]))); break;
        case 2: _t->progressUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->volumeChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 4: _t->playTrack((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->togglePlayPause(); break;
        case 6: _t->stop(); break;
        case 7: _t->next(); break;
        case 8: _t->prev(); break;
        case 9: _t->setShuffle((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 10: _t->setRepeatMode((*reinterpret_cast< std::add_pointer_t<RepeatMode>>(_a[1]))); break;
        case 11: _t->seek((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->setVolume((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 13: _t->onEngineStatusChange(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PlaybackService::*)(const Track & , int )>(_a, &PlaybackService::trackChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlaybackService::*)(sf::SoundSource::Status )>(_a, &PlaybackService::playbackStateChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlaybackService::*)(int , int )>(_a, &PlaybackService::progressUpdated, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlaybackService::*)(float )>(_a, &PlaybackService::volumeChanged, 3))
            return;
    }
}

const QMetaObject *PlaybackService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlaybackService::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15PlaybackServiceE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PlaybackService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void PlaybackService::trackChanged(const Track & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void PlaybackService::playbackStateChanged(sf::SoundSource::Status _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void PlaybackService::progressUpdated(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void PlaybackService::volumeChanged(float _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
