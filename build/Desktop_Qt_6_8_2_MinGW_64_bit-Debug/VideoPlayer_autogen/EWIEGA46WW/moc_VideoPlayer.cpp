/****************************************************************************
** Meta object code from reading C++ file 'VideoPlayer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../VideoPlayer.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoPlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
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
struct qt_meta_tag_ZN11VideoPlayerE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN11VideoPlayerE = QtMocHelpers::stringData(
    "VideoPlayer",
    "openDirectory",
    "",
    "onMusicListDoubleClicked",
    "QModelIndex",
    "index",
    "prevSong",
    "nextSong",
    "toggleVolume",
    "adjustVolume",
    "value",
    "playPause",
    "updatePlayButtonIcon",
    "QMediaPlayer::PlaybackState",
    "state",
    "updateDurationLab",
    "duration",
    "updatePlayDurLab",
    "position",
    "seekPosition",
    "autoSwitchToNext",
    "checkPlayCompletion",
    "onCurrentMediaChanged",
    "url"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN11VideoPlayerE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   98,    2, 0x08,    1 /* Private */,
       3,    1,   99,    2, 0x08,    2 /* Private */,
       6,    0,  102,    2, 0x08,    4 /* Private */,
       7,    0,  103,    2, 0x08,    5 /* Private */,
       8,    0,  104,    2, 0x08,    6 /* Private */,
       9,    1,  105,    2, 0x08,    7 /* Private */,
      11,    0,  108,    2, 0x08,    9 /* Private */,
      12,    1,  109,    2, 0x08,   10 /* Private */,
      15,    1,  112,    2, 0x08,   12 /* Private */,
      17,    1,  115,    2, 0x08,   14 /* Private */,
      19,    1,  118,    2, 0x08,   16 /* Private */,
      20,    0,  121,    2, 0x08,   18 /* Private */,
      21,    0,  122,    2, 0x08,   19 /* Private */,
      22,    1,  123,    2, 0x08,   20 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::LongLong,   16,
    QMetaType::Void, QMetaType::LongLong,   18,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QUrl,   23,

       0        // eod
};

Q_CONSTINIT const QMetaObject VideoPlayer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN11VideoPlayerE.offsetsAndSizes,
    qt_meta_data_ZN11VideoPlayerE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN11VideoPlayerE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VideoPlayer, std::true_type>,
        // method 'openDirectory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onMusicListDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'prevSong'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'nextSong'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleVolume'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'adjustVolume'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'playPause'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updatePlayButtonIcon'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMediaPlayer::PlaybackState, std::false_type>,
        // method 'updateDurationLab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'updatePlayDurLab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'seekPosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'autoSwitchToNext'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'checkPlayCompletion'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCurrentMediaChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QUrl &, std::false_type>
    >,
    nullptr
} };

void VideoPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<VideoPlayer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->openDirectory(); break;
        case 1: _t->onMusicListDoubleClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 2: _t->prevSong(); break;
        case 3: _t->nextSong(); break;
        case 4: _t->toggleVolume(); break;
        case 5: _t->adjustVolume((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->playPause(); break;
        case 7: _t->updatePlayButtonIcon((*reinterpret_cast< std::add_pointer_t<QMediaPlayer::PlaybackState>>(_a[1]))); break;
        case 8: _t->updateDurationLab((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 9: _t->updatePlayDurLab((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 10: _t->seekPosition((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->autoSwitchToNext(); break;
        case 12: _t->checkPlayCompletion(); break;
        case 13: _t->onCurrentMediaChanged((*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *VideoPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN11VideoPlayerE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VideoPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
