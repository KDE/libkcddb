add_definitions(-DTRANSLATION_DOMAIN="libkcddb")

set(KCDDB_INSTALL_INCLUDEDIR "${KDE_INSTALL_INCLUDEDIR}/KCddb6")
set(KCDDB_CMAKECONFIG_NAME "KCddb6")

ecm_setup_version(${libkcddb_VERSION}
    VARIABLE_PREFIX KCDDB
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kcddb_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${KCDDB_CMAKECONFIG_NAME}ConfigVersion.cmake"
    SOVERSION 5
)

add_library(KCddb SHARED)
add_library(KCddb6 ALIAS KCddb)

set_target_properties(KCddb PROPERTIES
    VERSION     ${KCDDB_VERSION}
    SOVERSION   ${KCDDB_SOVERSION}
    OUTPUT_NAME KCddb6
    EXPORT_NAME KCddb6
)


if(MUSICBRAINZ5_FOUND)
    set(musicbrainz_sources
       musicbrainz/musicbrainzlookup.cpp
       musicbrainz/asyncmusicbrainzlookup.cpp musicbrainz/asyncmusicbrainzlookup.h)

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
    cache.cpp cache.h
    cdinfo.cpp cdinfo.h
    config.cpp config.h
    client.cpp client.h
    kcddb.cpp kcddb.h
    cddb.cpp
    lookup.cpp
    cddbplookup.cpp cddbplookup.h
    synccddbplookup.cpp synccddbplookup.h
    asynccddbplookup.cpp asynccddbplookup.h
    httplookup.cpp httplookup.h
    synchttplookup.cpp
    asynchttplookup.cpp asynchttplookup.h
    submit.cpp
    sites.cpp sites.h
    httpsubmit.cpp
    asynchttpsubmit.cpp asynchttpsubmit.h
    synchttpsubmit.cpp
    categories.cpp categories.h
    genres.cpp genres.h
    ${musicbrainz_sources}
)

target_link_libraries(KCddb
    PUBLIC
        KF6::ConfigGui
    PRIVATE
        KF6::I18n
        KF6::KIOCore
        Qt6::Network
)

if(MUSICBRAINZ5_FOUND)
    target_link_libraries(KCddb PRIVATE musicbrainz)
endif()

# For Qt6/KF6 world transitively include the version header
set(_generate_export_header_version_args USE_VERSION_HEADER)

ecm_generate_export_header(KCddb
    BASE_NAME KCddb
    VERSION ${KCDDB_VERSION}
    DEPRECATED_BASE_VERSION 5.0.0
    DEPRECATION_VERSIONS 5.1
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
    ${_generate_export_header_version_args}
)

target_include_directories(KCddb
    PUBLIC
        "$<INSTALL_INTERFACE:${KCDDB_INSTALL_INCLUDEDIR}>"
)

ecm_generate_headers(KCddb_CamelCase_HEADERS
    HEADER_NAMES
        Categories
        CDInfo
        Client
        Genres
        Config
        KCDDB
    PREFIX KCDDB
    REQUIRED_HEADERS KCddb_HEADERS
)

install(TARGETS KCddb EXPORT KCddbTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kcddb_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/configbase.h
    ${KCddb_HEADERS}
    DESTINATION ${KCDDB_INSTALL_INCLUDEDIR}/kcddb # C++ namespace prefix
    COMPONENT Devel
)

install(FILES
    ${KCddb_CamelCase_HEADERS}
    DESTINATION ${KCDDB_INSTALL_INCLUDEDIR}/KCDDB # C++ namespace prefix
    COMPONENT Devel
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kcddb_version.h"
    DESTINATION "${KCDDB_INSTALL_INCLUDEDIR}"
    COMPONENT Devel
)

set(KCDDB_INCLUDE_DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KCddb6")

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
    COMPONENT Devel
)

include(ECMGeneratePriFile)
set(KCDDB_LIB_NAME libKCddb6)
ecm_generate_pri_file(
    BASE_NAME KCddb
    LIB_NAME ${KCDDB_LIB_NAME}
    DEPS "KConfigGui"
    FILENAME_VAR PRI_FILENAME
    INCLUDE_INSTALL_DIR ${KCDDB_INSTALL_INCLUDEDIR}
)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
