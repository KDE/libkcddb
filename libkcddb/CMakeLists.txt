add_definitions(-DTRANSLATION_DOMAIN="libkcddb")

set(KCDDB_INSTALL_INCLUDEDIR "${KDE_INSTALL_INCLUDEDIR_KF}/KCddb")
set(KCDDB_CMAKECONFIG_NAME "KF5Cddb")

ecm_setup_version(${libkcddb_VERSION}
    VARIABLE_PREFIX KCDDB
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kcddb_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${KCDDB_CMAKECONFIG_NAME}ConfigVersion.cmake"
    SOVERSION 5
)

add_library(KCddb SHARED)
add_library(KF5::Cddb ALIAS KCddb)

if(MUSICBRAINZ5_FOUND)
    set(musicbrainz_sources
       musicbrainz/musicbrainzlookup.cpp
       musicbrainz/asyncmusicbrainzlookup.cpp)

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" OR (WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"))
        set(enable_exceptions -EHsc)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(enable_exceptions "-fexceptions -UQT_NO_EXCEPTIONS")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        set(enable_exceptions -fexceptions)
    endif()

    set_source_files_properties(${musicbrainz_sources} PROPERTIES COMPILE_FLAGS ${enable_exceptions})
endif()

kconfig_add_kcfg_files(KCddb configbase.kcfgc)

ecm_qt_declare_logging_category(KCddb
    HEADER logging.h
    IDENTIFIER LIBKCDDB
    CATEGORY_NAME "libkcddb"
    DESCRIPTION "libkcddb"
    EXPORT KCDDB
)

target_sources(KCddb PRIVATE
    cache.cpp
    cdinfo.cpp
    kcddbconfig.cpp
    client.cpp
    kcddb.cpp
    cddb.cpp
    lookup.cpp
    cddbplookup.cpp
    synccddbplookup.cpp
    asynccddbplookup.cpp
    httplookup.cpp
    synchttplookup.cpp
    asynchttplookup.cpp
    smtpsubmit.cpp
    asyncsmtpsubmit.cpp
    syncsmtpsubmit.cpp
    submit.cpp
    sites.cpp
    httpsubmit.cpp
    asynchttpsubmit.cpp
    synchttpsubmit.cpp
    categories.cpp
    genres.cpp
    ${musicbrainz_sources}
)

target_link_libraries(KCddb
    PUBLIC
        KF5::ConfigGui
    PRIVATE
        KF5::I18n
        KF5::KIOCore
        Qt${QT_MAJOR_VERSION}::Network
)

if(MUSICBRAINZ5_FOUND)
    target_link_libraries(KCddb PRIVATE musicbrainz)
endif()

generate_export_header(KCddb BASE_NAME KCddb)
target_include_directories(KCddb
    PUBLIC
        "$<INSTALL_INTERFACE:${KCDDB_INSTALL_INCLUDEDIR}>"
)

set_target_properties(KCddb PROPERTIES
    VERSION ${KCDDB_VERSION}
    SOVERSION ${KCDDB_SOVERSION}
    OUTPUT_NAME KF5Cddb
    EXPORT_NAME Cddb
)

ecm_generate_headers(KCddb_HEADERS
    HEADER_NAMES
        Categories
        Cdinfo
        Client
        Genres
        KcddbConfig
        Kcddb
    REQUIRED_HEADERS KCddb_HEADERS
)

install(TARGETS KCddb EXPORT KCddbTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kcddb_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/configbase.h
    ${KCddb_HEADERS}
    DESTINATION ${KCDDB_INSTALL_INCLUDEDIR}
    COMPONENT Devel
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kcddb_version.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF}"
    COMPONENT Devel
)

install(FILES libkcddb5.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})

ecm_qt_install_logging_categories(
    EXPORT KCDDB
    FILE libkcddb.categories
    DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}"
)

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/${KCDDB_CMAKECONFIG_NAME}")
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/${KCDDB_CMAKECONFIG_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${KCDDB_CMAKECONFIG_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${KCDDB_CMAKECONFIG_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${KCDDB_CMAKECONFIG_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KCddbTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE ${KCDDB_CMAKECONFIG_NAME}Targets.cmake
    NAMESPACE KF5::
    COMPONENT Devel
)

include(ECMGeneratePriFile)
ecm_generate_pri_file(
    BASE_NAME KCddb
    LIB_NAME KF5Cddb
    DEPS "KConfigGui"
    FILENAME_VAR PRI_FILENAME
    INCLUDE_INSTALL_DIR ${KCDDB_INSTALL_INCLUDEDIR}
)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
