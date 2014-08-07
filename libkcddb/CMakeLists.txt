
if(MUSICBRAINZ5_FOUND)
    set(musicbrainz_sources
       musicbrainz/musicbrainzlookup.cpp
       musicbrainz/asyncmusicbrainzlookup.cpp)

    include_directories(${MUSICBRAINZ5_INCLUDE_DIR})

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" OR (WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"))
        set(enable_exceptions -EHsc)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(enable_exceptions "-fexceptions -UQT_NO_EXCEPTIONS")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        set(enable_exceptions -fexceptions)
    endif()

    set_source_files_properties(${musicbrainz_sources} PROPERTIES COMPILE_FLAGS ${enable_exceptions})
endif()

qt5_wrap_ui(ui_sources cdinfodialog.ui cdinfoencodingwidget.ui)

kconfig_add_kcfg_files(kcfg_sources configbase.kcfgc)

add_library(kcddb SHARED
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
    cdinfodialog.cpp
    cdinfoencodingwidget.cpp
    logging.cpp
    ${musicbrainz_sources}
    ${ui_sources}
    ${kcfg_sources})

target_link_libraries(kcddb
    PUBLIC
        KF5::ConfigGui
        Qt5::Widgets
    PRIVATE
        KF5::Codecs
        KF5::I18n
        KF5::KIOCore
        Qt5::Network
        ${MUSICBRAINZ5_LIBRARIES})

generate_export_header(kcddb)

set_target_properties(kcddb PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION})

install(TARGETS kcddb EXPORT LibkcddbTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
   cdinfo.h
   cdinfodialog.h
   client.h
   genres.h
   kcddbconfig.h
   kcddb.h
   ${CMAKE_CURRENT_BINARY_DIR}/kcddb_export.h
   ${CMAKE_CURRENT_BINARY_DIR}/configbase.h
   DESTINATION ${INCLUDE_INSTALL_DIR}/libkcddb COMPONENT Devel)

install( FILES libkcddb.kcfg DESTINATION ${KCFG_INSTALL_DIR})
