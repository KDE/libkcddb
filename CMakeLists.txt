cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

project(libkcddb VERSION "5.0.0")

set(QT_MIN_VERSION 5.12.0)
set(KF_MIN_VERSION 5.68.0)

# ECM setup
include(FeatureSummary)
find_package(ECM ${KF_MIN_VERSION}  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
set(CMAKE_MODULE_PATH "${ECM_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(GenerateExportHeader)
include(ECMSetupVersion)
include(ECMQtDeclareLoggingCategory)
include(CMakePackageConfigHelpers)
include(ECMGenerateHeaders)

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS Network Widgets)
find_package(KF5 ${KF_MIN_VERSION} REQUIRED COMPONENTS Config DocTools I18n KIO WidgetsAddons)

ecm_setup_version(${libkcddb_VERSION}
    VARIABLE_PREFIX KCDDB
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kcddb_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5CddbConfigVersion.cmake"
    SOVERSION 5
)

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

add_definitions(
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_SIGNALS_SLOTS_KEYWORDS
    -DQT_USE_QSTRINGBUILDER
    -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
    -DQT_STRICT_ITERATORS
)

configure_file(config-musicbrainz.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-musicbrainz.h )
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(kcmcddb)
add_subdirectory(libkcddb)

if(BUILD_TESTING)
    find_package(Qt5Test REQUIRED)
    add_subdirectory(tests)
endif()

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Cddb")
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/KF5CddbConfig.cmake.in"
                              "${CMAKE_CURRENT_BINARY_DIR}/KF5CddbConfig.cmake"
                              INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5CddbConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5CddbConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT
    KF5CddbTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KF5CddbTargets.cmake
    NAMESPACE KF5::
    COMPONENT Devel
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kcddb_version.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}"
    COMPONENT Devel
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
