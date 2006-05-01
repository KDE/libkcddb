if build_MB
  MB_DIR = musicbrainz
  mb_lib = $(top_builddir)/libkcddb/musicbrainz/libmusicbrainz.la
endif



SUBDIRS = $(MB_DIR) . test kcmcddb

INCLUDES = -I$(srcdir)/.. $(all_includes)

lib_LTLIBRARIES = libkcddb.la

libkcddb_la_SOURCES = \
    cache.cpp cdinfo.cpp kcddbconfig.cpp client.cpp cddb.cpp lookup.cpp \
    cddbplookup.cpp synccddbplookup.cpp asynccddbplookup.cpp httplookup.cpp \
    synchttplookup.cpp asynchttplookup.cpp smtpsubmit.cpp \
    asyncsmtpsubmit.cpp syncsmtpsubmit.cpp configbase.kcfgc \
    submit.cpp sites.cpp httpsubmit.cpp asynchttpsubmit.cpp \
    synchttpsubmit.cpp categories.cpp genres.cpp \
    cdinfodialog.cpp cdinfodialogbase.ui cdinfoencodingwidget.cpp \
    cdinfoencodingwidgetbase.ui

libkcddb_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
libkcddb_la_LIBADD  = $(LIB_KDECORE) $(LIB_KIO) $(mb_lib)
    

METASOURCES = AUTO

kde_kcfg_DATA = libkcddb.kcfg

kcddbincludedir = $(includedir)/libkcddb
kcddbinclude_HEADERS = \
    cdinfo.h client.h kcddbconfig.h cddb.h configbase.h


kcddbconfig.lo: configbase.h
