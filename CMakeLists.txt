project(libkcddb)

add_subdirectory( test )
add_subdirectory( kcmcddb )

macro_optional_find_package(MusicBrainz)

if(MUSICBRAINZ_FOUND)
    set(HAVE_MUSICBRAINZ 1)
    set(libmusicbrainz_SRCS ${libmusicbrainz_SRCS}
       musicbrainz/musicbrainzlookup.cpp
       musicbrainz/asyncmusicbrainzlookup.cpp)
else(MUSICBRAINZ_FOUND)
    set(HAVE_MUSICBRAINZ 0)
    set(libmusicbrainz_SRCS)
endif(MUSICBRAINZ_FOUND)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-musicbrainz.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-musicbrainz.h )

include_directories(${CMAKE_BINARY_DIR}/libkcddb/ )

add_definitions (-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)

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

target_link_libraries(kcddb ${KDE4_KDE3SUPPORT_LIBS})
if(MUSICBRAINZ_FOUND)
    target_link_libraries(kcddb musicbrainz)
    include_directories(${MUSICBRAINZ_INCLUDE_DIR})
endif(MUSICBRAINZ_FOUND)

set_target_properties(kcddb PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS kcddb  DESTINATION ${LIB_INSTALL_DIR} )


########### install files ###############

install( FILES cdinfo.h client.h kcddbconfig.h kcddb.h ${CMAKE_CURRENT_BINARY_DIR}/configbase.h  DESTINATION ${INCLUDE_INSTALL_DIR}/libkcddb)
install( FILES libkcddb.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )



