cmake_minimum_required (VERSION 3.5 FATAL_ERROR)
project(libkcddb)


# ECM setup
include(FeatureSummary)
set(KF5_DEP_VERSION "5.24.0") # handled by release scripts
find_package(ECM ${KF5_DEP_VERSION}  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
set(CMAKE_MODULE_PATH "${ECM_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(GenerateExportHeader)
include(ECMSetupVersion)
include(ECMPackageConfigHelpers)
include(ECMGenerateHeaders)

set(PROJECT_VERSION "16.08.0")
set(REQUIRED_QT_VERSION 5.4.0)

find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Network Widgets)
find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS Config DocTools Codecs I18n KIO WidgetsAddons)

ecm_setup_version("5.0.0"
    VARIABLE_PREFIX KCDDB
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kcddb_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5CddbConfigVersion.cmake"
    SOVERSION 5
)

find_package(MusicBrainz5)
set_package_properties(MusicBrainz5 PROPERTIES
   DESCRIPTION "Music metadata lookup for KDE multimedia applications through libkcddb. You need version 5.x of libmusicbrainz"
   URL "http://www.musicbrainz.org"
   TYPE OPTIONAL
   PURPOSE "A library that provides access to metadata lookup on the MusicBrainz server")

if(MUSICBRAINZ5_FOUND)
    set(HAVE_MUSICBRAINZ5 1)
else()
    set(HAVE_MUSICBRAINZ5 0)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-musicbrainz.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-musicbrainz.h )

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
   add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
endif()

add_subdirectory(kcmcddb)
add_subdirectory(libkcddb)

option(BUILD_TESTING "Build test" OFF)
if(BUILD_TESTING)
    find_package(Qt5Test REQUIRED)
    add_subdirectory(tests)
endif()

feature_summary(WHAT PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5Cddb")
ecm_configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/KF5CddbConfig.cmake.in"
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
