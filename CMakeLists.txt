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
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(GenerateExportHeader)
include(ECMSetupVersion)
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

configure_file(config-musicbrainz.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-musicbrainz.h )
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(kcmcddb)
add_subdirectory(libkcddb)

if(BUILD_TESTING)
    find_package(Qt5Test REQUIRED)
    add_subdirectory(tests)
endif()

set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5Cddb")
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
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}"
    COMPONENT Devel
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
