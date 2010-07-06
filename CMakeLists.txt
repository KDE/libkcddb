project(libkcddb)

add_subdirectory( test )
add_subdirectory( kcmcddb )

if(MUSICBRAINZ3_FOUND)
    set(HAVE_MUSICBRAINZ3 1)
    include_directories(${MUSICBRAINZ3_INCLUDE_DIR})

    set(libmusicbrainz_SRCS ${libmusicbrainz_SRCS}
       musicbrainz/musicbrainzlookup.cpp
       musicbrainz/asyncmusicbrainzlookup.cpp)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")
else(MUSICBRAINZ3_FOUND)
    set(HAVE_MUSICBRAINZ3 0)
    set(libmusicbrainz_SRCS)
endif(MUSICBRAINZ3_FOUND)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-musicbrainz.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-musicbrainz.h )

include_directories(${CMAKE_BINARY_DIR}/libkcddb/ )

########### next target ###############

set(kcddb_LIB_SRCS ${libmusicbrainz_SRCS}
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
   cdinfoencodingwidget.cpp )


kde4_add_ui_files(kcddb_LIB_SRCS cdinfodialog.ui cdinfoencodingwidget.ui )

kde4_add_kcfg_files(kcddb_LIB_SRCS configbase.kcfgc )

kde4_add_library(kcddb SHARED ${kcddb_LIB_SRCS})

target_link_libraries(kcddb ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
if(MUSICBRAINZ3_FOUND)
    target_link_libraries(kcddb ${MUSICBRAINZ3_LIBRARIES})
endif(MUSICBRAINZ3_FOUND)

set_target_properties(kcddb PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS kcddb  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install(FILES
   cdinfo.h
   cdinfodialog.h
   client.h
   genres.h
   kcddbconfig.h
   kcddb.h
   kcddb_export.h
   ${CMAKE_CURRENT_BINARY_DIR}/configbase.h
   DESTINATION ${INCLUDE_INSTALL_DIR}/libkcddb COMPONENT Devel )
install( FILES libkcddb.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )



