cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# KDE Applications version, managed by release script.
set(RELEASE_SERVICE_VERSION_MAJOR "24")
set(RELEASE_SERVICE_VERSION_MINOR "04")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(libkcddb VERSION ${RELEASE_SERVICE_VERSION})

set(QT_MIN_VERSION 5.15.2)
set(KF_MIN_VERSION 5.92.0)

# ECM setup
include(FeatureSummary)
find_package(ECM ${KF_MIN_VERSION}  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
set(CMAKE_MODULE_PATH "${ECM_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMGenerateExportHeader)
include(ECMSetupVersion)
include(ECMQtDeclareLoggingCategory)
include(CMakePackageConfigHelpers)
include(ECMGenerateHeaders)
include(ECMDeprecationSettings)
if (QT_MAJOR_VERSION STREQUAL "6")
    set(QT_REQUIRED_VERSION "6.5.0")
    set(KF_MIN_VERSION "5.240.0")
    set(KF_MAJOR_VERSION "6")
else()
    set(KF_MAJOR_VERSION "5")
endif()

# reset to 0 once all deprecated API has been removed on full switch to Qt6
if (QT_MAJOR_VERSION STREQUAL "6")
set(EXCLUDE_DEPRECATED_BEFORE_AND_AT 5.1.0 CACHE STRING "Control the range of deprecated API excluded from the build [default=5.1.0].")
else()
set(EXCLUDE_DEPRECATED_BEFORE_AND_AT 0 CACHE STRING "Control the range of deprecated API excluded from the build [default=0].")
endif()

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} REQUIRED COMPONENTS Network Widgets)
find_package(KF${KF_MAJOR_VERSION} ${KF_MIN_VERSION} REQUIRED COMPONENTS Config DocTools I18n KIO WidgetsAddons KCMUtils)
find_package(MusicBrainz5)
set_package_properties(MusicBrainz5 PROPERTIES
   DESCRIPTION "Music metadata lookup for KDE multimedia applications through libkcddb. You need version 5.x of libmusicbrainz"
   URL "https://www.musicbrainz.org"
   TYPE OPTIONAL
   PURPOSE "A library that provides access to metadata lookup on the MusicBrainz server")

if(MUSICBRAINZ5_FOUND)
    set(HAVE_MUSICBRAINZ5 1)
else()
    set(HAVE_MUSICBRAINZ5 0)
endif()

if (QT_MAJOR_VERSION STREQUAL "6")
    ecm_set_disabled_deprecation_versions(
        QT 6.5
        KF 5.239
    )
else()
    ecm_set_disabled_deprecation_versions(
        QT 5.15.2
        KF 5.101
    )
endif()

configure_file(config-musicbrainz.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-musicbrainz.h )
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(kcmcddb)
add_subdirectory(libkcddb)

if(BUILD_TESTING)
    find_package(Qt${QT_MAJOR_VERSION}Test REQUIRED)
    add_subdirectory(tests)
endif()

ki18n_install(po)
kdoctools_install(po)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
