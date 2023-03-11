add_definitions(-DTRANSLATION_DOMAIN="libkcddb")

set(KCDDB_INSTALL_INCLUDEDIR "${KDE_INSTALL_INCLUDEDIR}/KCddb${QT_MAJOR_VERSION}")
if (KF_MAJOR_VERSION STREQUAL "5")
    set(KCDDB_CMAKECONFIG_NAME "KF5Cddb")
else()
    set(KCDDB_CMAKECONFIG_NAME "KCddb6")
endif()

ecm_setup_version(${libkcddb_VERSION}
    VARIABLE_PREFIX KCDDB
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kcddb_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${KCDDB_CMAKECONFIG_NAME}ConfigVersion.cmake"
    SOVERSION 5
)

add_library(KCddb SHARED)
if (KF_MAJOR_VERSION STREQUAL "5")
    add_library(KF5::Cddb ALIAS KCddb)
else()
    add_library(KCddb6 ALIAS KCddb)
endif()

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
    smtpsubmit.cpp smtpsubmit.h
    asyncsmtpsubmit.cpp asyncsmtpsubmit.h
    syncsmtpsubmit.cpp syncsmtpsubmit.h
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
        KF${KF_MAJOR_VERSION}::ConfigGui
    PRIVATE
        KF${KF_MAJOR_VERSION}::I18n
        KF${KF_MAJOR_VERSION}::KIOCore
        Qt${QT_MAJOR_VERSION}::Network
)

if (TARGET Qt6::Core5Compat)
    target_link_libraries(KCddb PRIVATE Qt6::Core5Compat)
endif()

if(MUSICBRAINZ5_FOUND)
    target_link_libraries(KCddb PRIVATE musicbrainz)
endif()

ecm_generate_export_header(KCddb
    BASE_NAME KCddb
    VERSION ${KCDDB_VERSION}
    DEPRECATED_BASE_VERSION 5.0.0
    DEPRECATION_VERSIONS 5.1
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

target_include_directories(KCddb
    PUBLIC
        "$<INSTALL_INTERFACE:${KCDDB_INSTALL_INCLUDEDIR}>"
)


if (KF_MAJOR_VERSION STREQUAL "5")
    set_target_properties(KCddb PROPERTIES
        VERSION ${KCDDB_VERSION}
        SOVERSION ${KCDDB_SOVERSION}
        OUTPUT_NAME KF${KF_MAJOR_VERSION}Cddb
        EXPORT_NAME Cddb
    )
else()
    set_target_properties(KCddb PROPERTIES
        VERSION ${KCDDB_VERSION}
        SOVERSION ${KCDDB_SOVERSION}
        OUTPUT_NAME KCddb6
        EXPORT_NAME Cddb6
    )
endif()

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

if (KF_MAJOR_VERSION STREQUAL "6")
    install(TARGETS KCddb EXPORT KCddbTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
else()
    install(TARGETS KCddb EXPORT KCddbTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})
endif()

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

if (KF_MAJOR_VERSION STREQUAL "5")
    set(KCDDB_INCLUDE_DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF}/KCddb")
else()
    set(KCDDB_INCLUDE_DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KCddb6")
endif()
# provide compat headers for old broken include path prefix KCddb/
if(NOT EXCLUDE_DEPRECATED_BEFORE_AND_AT STREQUAL "CURRENT" AND
   EXCLUDE_DEPRECATED_BEFORE_AND_AT VERSION_LESS 5.1.0)
    function(generate_compat_headers)
        foreach(_classnameentry ${ARGV})
            string(REPLACE "," ";" _classnames ${_classnameentry})
            list(GET _classnames 0 classname)
            list(GET _classnames -1 newclassname) # last is either second or the only one

            string(TOLOWER ${classname} classname_lc)
            string(TOLOWER ${newclassname} newclassname_lc)
            set(NEWHEADER_NAME "${newclassname_lc}.h")
            # normal header
            set(compat_header "${CMAKE_CURRENT_BINARY_DIR}/compat/${classname_lc}.h")
            set(NEW_INCLUDE "kcddb/${NEWHEADER_NAME}")
            configure_file(compatheader.h.in ${compat_header} @ONLY)
            install(FILES ${compat_header}
                DESTINATION ${KCDDB_INCLUDE_DESTINATION}
                COMPONENT Devel
            )
            # CamelCase header
            set(compat_header "${CMAKE_CURRENT_BINARY_DIR}/compat/${classname}")
            set(NEW_INCLUDE "KCDDB/${newclassname}")
            configure_file(compatheader.h.in ${compat_header} @ONLY)
            install(FILES ${compat_header}
                DESTINATION ${KCDDB_INCLUDE_DESTINATION}
                COMPONENT Devel
            )
        endforeach()
    endfunction()

    generate_compat_headers(
        Categories
        Cdinfo,CDInfo
        Client
        Genres
        KcddbConfig,Config
        Kcddb,KCDDB
    )
endif()

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
if (KF_MAJOR_VERSION STREQUAL "5")
    set(KCDDB_LIB_NAME KF${KF_MAJOR_VERSION}Cddb)
else()
    set(KCDDB_LIB_NAME libKCddb6)
endif()
ecm_generate_pri_file(
    BASE_NAME KCddb
    LIB_NAME ${KCDDB_LIB_NAME}
    DEPS "KConfigGui"
    FILENAME_VAR PRI_FILENAME
    INCLUDE_INSTALL_DIR ${KCDDB_INSTALL_INCLUDEDIR}
)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
