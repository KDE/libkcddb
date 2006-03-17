kde4_header()
project(libkcddb)

add_subdirectory( test ) 
add_subdirectory( kcmcddb ) 

message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}: skipped subdir $(MB_DIR)")
include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############
set(libmusicbrainz_SRCS 
	${CMAKE_SOURCE_DIR}/libkcddb/musicbrainz/musicbrainzlookup.cpp 
	${CMAKE_SOURCE_DIR}/libkcddb/musicbrainz/sha1.c 
	)

set(kcddb_LIB_SRCS ${libmusicbrainz_SRCS}
   cache.cpp 
   cdinfo.cpp 
   config.cpp 
   client.cpp 
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

kde4_automoc(${kcddb_LIB_SRCS})

kde4_add_ui_files(kcddb_LIB_SRCS cdinfodialogbase.ui cdinfoencodingwidgetbase.ui )

kde4_add_kcfg_files(kcddb_LIB_SRCS configbase.kcfgc )

kde4_add_library(kcddb SHARED ${kcddb_LIB_SRCS})

target_link_libraries(kcddb  ${LIB_KDE3SUPPORT} musicbrainz )

set_target_properties(kcddb PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} kcddb )


########### install files ###############

install_files( /include/libkcddb FILES cdinfo.h client.h config.h cddb.h configbase.h )
install_files( ${KCFG_INSTALL_DIR} FILES libkcddb.kcfg )

kde4_footer()



#original Makefile.am contents follow:

#if build_MB
#  MB_DIR = musicbrainz
#  mb_lib = $(top_builddir)/libkcddb/musicbrainz/libmusicbrainz.la
#endif
#
#
#
#SUBDIRS = $(MB_DIR) . test kcmcddb
#
#INCLUDES = -I$(srcdir)/.. $(all_includes)
#
#lib_LTLIBRARIES = libkcddb.la
#
#libkcddb_la_SOURCES = \
#    cache.cpp cdinfo.cpp config.cpp client.cpp cddb.cpp lookup.cpp \
#    cddbplookup.cpp synccddbplookup.cpp asynccddbplookup.cpp httplookup.cpp \
#    synchttplookup.cpp asynchttplookup.cpp smtpsubmit.cpp \
#    asyncsmtpsubmit.cpp syncsmtpsubmit.cpp configbase.kcfgc \
#    submit.cpp sites.cpp httpsubmit.cpp asynchttpsubmit.cpp \
#    synchttpsubmit.cpp categories.cpp genres.cpp \
#    cdinfodialog.cpp cdinfodialogbase.ui cdinfoencodingwidget.cpp \
#    cdinfoencodingwidgetbase.ui
#
#libkcddb_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
#libkcddb_la_LIBADD  = $(LIB_KDECORE) $(LIB_KIO) $(mb_lib)
#    
#
#METASOURCES = AUTO
#
#kde_kcfg_DATA = libkcddb.kcfg
#
#kcddbincludedir = $(includedir)/libkcddb
#kcddbinclude_HEADERS = \
#    cdinfo.h client.h config.h cddb.h configbase.h
#
#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/libkcddb.pot
#
#config.lo: configbase.h