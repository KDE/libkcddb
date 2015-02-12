project(libkcddb)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${ECM_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
include(FeatureSummary)
include(GenerateExportHeader)
include(ECMSetupVersion)

find_package(Qt5 REQUIRED COMPONENTS Network Widgets)
find_package(KF5 REQUIRED COMPONENTS Config DocTools Codecs I18n KIO WidgetsAddons)

ecm_setup_version(${KF5_VERSION}
    VARIABLE_PREFIX KCDDB
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/LibkcddbConfigVersion.cmake")

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

add_subdirectory( kcmcddb )
add_subdirectory( libkcddb )

if(BUILD_TESTING)
    find_package(Qt5Test REQUIRED)
    add_subdirectory( test )
endif()

feature_summary(WHAT PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

# Figure out the relative path from the installed Config.cmake file to the install prefix (which may be at
# runtime different from the chosen CMAKE_INSTALL_PREFIX if under Windows the package was installed anywhere)
# This relative path will be configured into LibkcddbConfig.cmake
file(RELATIVE_PATH relInstallDir ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/cmake/libkcddb ${CMAKE_INSTALL_PREFIX})

configure_file(LibkcddbConfig.cmake.in ${CMAKE_BINARY_DIR}/LibkcddbConfig.cmake @ONLY)
install(FILES
    ${CMAKE_BINARY_DIR}/LibkcddbConfig.cmake
    ${CMAKE_BINARY_DIR}/LibkcddbConfigVersion.cmake
    DESTINATION ${LIB_INSTALL_DIR}/cmake/libkcddb)
install(EXPORT LibkcddbTargets DESTINATION ${LIB_INSTALL_DIR}/cmake/libkcddb/ FILE LibkcddbTargets.cmake NAMESPACE KDE::Libkcddb::)
