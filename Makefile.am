SUBDIRS = . kcmcddb

INCLUDES = -I$(srcdir)/.. $(all_includes)

lib_LTLIBRARIES = libkcddb.la

libkcddb_la_SOURCES = defines.cpp config.cpp client.cpp
libkcddb_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -module
libkcddb_la_LIBADD  = $(LIB_KDECORE)

kcddbincludedir = $(includedir)/libkcddb
kcddbinclude_HEADERS = defines.h config.h client.h

noinst_PROGRAMS = test

test_SOURCES = test.cpp

test_LDFLAGS = $(all_libraries)
test_LDADD = libkcddb.la $(LIB_KDECORE)
