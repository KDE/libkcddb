SUBDIRS = . kcmcddb test

INCLUDES = -I$(srcdir)/.. $(all_includes)

lib_LTLIBRARIES = libkcddb.la

libkcddb_la_SOURCES = \
    cache.cpp cdinfo.cpp config.cpp client.cpp cddb.cpp lookup.cpp \
    cddbplookup.cpp synccddbplookup.cpp asynccddbplookup.cpp httplookup.cpp \
    synchttplookup.cpp asynchttplookup.cpp smtpsubmit.cpp \
    asyncsmtpsubmit.cpp syncsmtpsubmit.cpp configbase.kcfgc \
    submit.cpp sites.cpp httpsubmit.cpp asynchttpsubmit.cpp \
    synchttpsubmit.cpp \
    cdinfodialogbase.ui categories.cpp genres.cpp

libkcddb_la_LDFLAGS = $(all_libraries) -version-info 1:0:0
libkcddb_la_LIBADD  = $(LIB_KDECORE) $(LIB_KIO)

METASOURCES = AUTO

kde_kcfg_DATA = libkcddb.kcfg

kcddbincludedir = $(includedir)/libkcddb
kcddbinclude_HEADERS = \
    cache.h cdinfo.h client.h config.h cddb.h configbase.h

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/libkcddb.pot
