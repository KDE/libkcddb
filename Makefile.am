SUBDIRS = . kcmcddb test

INCLUDES = -I$(srcdir)/.. $(all_includes)

lib_LTLIBRARIES = libkcddb.la

libkcddb_la_SOURCES =\
cache.cpp\
cdinfo.cpp\
config.cpp\
client.cpp\
lookup.cpp\
cddbplookup.cpp\
submit.cpp\
synccddbplookup.cpp\
asynccddbplookup.cpp\
httplookup.cpp\
synchttplookup.cpp

libkcddb_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -module
libkcddb_la_LIBADD  = $(LIB_KDECORE) $(LIB_KIO)

METASOURCES = AUTO

kcddbincludedir = $(includedir)/libkcddb
kcddbinclude_HEADERS =\
cache.h\
cdinfo.h\
client.h\
config.h\
lookup.h\
cddbplookup.h\
submit.h\
synccddbplookup.h\
asynccddbplookup.h\
httplookup.h\
synchttplookup.h
