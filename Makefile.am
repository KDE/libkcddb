SUBDIRS = . kcmcddb test

INCLUDES = -I$(srcdir)/.. $(all_includes)

lib_LTLIBRARIES = libkcddb.la

libkcddb_la_SOURCES = defines.cpp config.cpp client.cpp asyncclient.cpp asynccddblookup.cpp asynchttplookup.cpp synccddblookup.cpp
libkcddb_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -module
libkcddb_la_LIBADD  = $(LIB_KDECORE) $(LIB_KIO)

METASOURCES = AUTO

kcddbincludedir = $(includedir)/libkcddb
kcddbinclude_HEADERS = defines.h config.h client.h asyncclient.h


