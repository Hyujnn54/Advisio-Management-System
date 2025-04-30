/****************************************************************************
** Meta object code from reading C++ file 'meetingmanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../managers/meeting/meetingmanager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'meetingmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMeetingManagerENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMeetingManagerENDCLASS = QtMocHelpers::stringData(
    "MeetingManager",
    "handleAddButtonClick",
    "",
    "handleDeleteButtonClick",
    "handleUpdateButtonClick",
    "handleSearchButtonClick",
    "handleSearchTextChanged",
    "text",
    "handleSortCriteriaChanged",
    "index",
    "handleGenerateQRCodeButtonClick",
    "handleExportPdfButtonClick",
    "handleStatisticsButtonClick",
    "handleRefreshStatsButtonClick",
    "handleTabChanged",
    "updateInputFields"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMeetingManagerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x0a,    1 /* Public */,
       3,    0,   87,    2, 0x0a,    2 /* Public */,
       4,    0,   88,    2, 0x0a,    3 /* Public */,
       5,    0,   89,    2, 0x0a,    4 /* Public */,
       6,    1,   90,    2, 0x0a,    5 /* Public */,
       8,    1,   93,    2, 0x0a,    7 /* Public */,
      10,    0,   96,    2, 0x0a,    9 /* Public */,
      11,    0,   97,    2, 0x0a,   10 /* Public */,
      12,    0,   98,    2, 0x0a,   11 /* Public */,
      13,    0,   99,    2, 0x0a,   12 /* Public */,
      14,    1,  100,    2, 0x0a,   13 /* Public */,
      15,    0,  103,    2, 0x0a,   15 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MeetingManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSMeetingManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMeetingManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMeetingManagerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MeetingManager, std::true_type>,
        // method 'handleAddButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleDeleteButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleUpdateButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleSearchButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleSearchTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'handleSortCriteriaChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'handleGenerateQRCodeButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleExportPdfButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleStatisticsButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleRefreshStatsButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleTabChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateInputFields'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MeetingManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MeetingManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->handleAddButtonClick(); break;
        case 1: _t->handleDeleteButtonClick(); break;
        case 2: _t->handleUpdateButtonClick(); break;
        case 3: _t->handleSearchButtonClick(); break;
        case 4: _t->handleSearchTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->handleSortCriteriaChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->handleGenerateQRCodeButtonClick(); break;
        case 7: _t->handleExportPdfButtonClick(); break;
        case 8: _t->handleStatisticsButtonClick(); break;
        case 9: _t->handleRefreshStatsButtonClick(); break;
        case 10: _t->handleTabChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->updateInputFields(); break;
        default: ;
        }
    }
}

const QMetaObject *MeetingManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MeetingManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMeetingManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MeetingManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
