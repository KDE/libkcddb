cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(libkcddb VERSION "5.1.0")

set(QT_MIN_VERSION 6.6.2)
set(KF_MIN_VERSION 6.7.0)

# ECM setup
include(FeatureSummary)
find_package(ECM ${KF_MIN_VERSION}  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
set(CMAKE_MODULE_PATH "${ECM_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_DOC "Whether to build the documentation" ON)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMGenerateExportHeader)
include(ECMSetupVersion)
include(ECMQtDeclareLoggingCategory)
include(CMakePackageConfigHelpers)
include(ECMGenerateHeaders)
include(ECMDeprecationSettings)

# reset to 0 once all deprecated API has been removed on full switch to Qt6
set(EXCLUDE_DEPRECATED_BEFORE_AND_AT 5.1.0 CACHE STRING "Control the range of deprecated API excluded from the build [default=5.1.0].")

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS Network Widgets)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS Config I18n KIO WidgetsAddons KCMUtils)
if(BUILD_DOC)
    find_package(KF6DocTools ${KF_MIN_VERSION})
    set_package_properties(KF6DocTools PROPERTIES
        DESCRIPTION "Tools to generate documentation"
        TYPE REQUIRED
    )
endif()
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

ecm_set_disabled_deprecation_versions(
    QT 6.5
    KF 5.239
)

configure_file(config-musicbrainz.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-musicbrainz.h )
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(kcmcddb)
add_subdirectory(libkcddb)

if(BUILD_TESTING)
    find_package(Qt6Test REQUIRED)
    add_subdirectory(tests)
endif()

ki18n_install(po)

if(BUILD_DOC)
    kdoctools_install(po)
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
