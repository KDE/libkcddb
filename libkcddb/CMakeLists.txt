add_definitions(-DTRANSLATION_DOMAIN="libkcddb")

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

kconfig_add_kcfg_files(kcfg_sources configbase.kcfgc)

ecm_qt_declare_logging_category(log_sources
    HEADER logging.h
    IDENTIFIER LIBKCDDB
    CATEGORY_NAME "libkcddb"
    DESCRIPTION "libkcddb"
    EXPORT KCDDB
)

add_library(KF5Cddb SHARED
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
    ${log_sources}
    ${kcfg_sources})

target_link_libraries(KF5Cddb
    PUBLIC
        KF5::ConfigGui
    PRIVATE
        KF5::I18n
        KF5::KIOCore
        Qt5::Network)

if(MUSICBRAINZ5_FOUND)
    target_link_libraries(KF5Cddb PRIVATE musicbrainz)
endif()

generate_export_header(KF5Cddb BASE_NAME KCddb)
add_library(KF5::Cddb ALIAS KF5Cddb)
target_include_directories(KF5Cddb PUBLIC "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KCddb>")

set_target_properties(KF5Cddb PROPERTIES VERSION ${KCDDB_VERSION_STRING} 
    SOVERSION ${KCDDB_SOVERSION}
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

install(TARGETS KF5Cddb EXPORT KF5CddbTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kcddb_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/configbase.h
    ${KCddb_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KCddb COMPONENT Devel
)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KCddb LIB_NAME KF5Cddb DEPS "Qt5Widgets" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KCddb)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

install(FILES libkcddb5.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})

ecm_qt_install_logging_categories(
    EXPORT KCDDB
    FILE libkcddb.categories
    DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}"
)
