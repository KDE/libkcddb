SUBDIRS = . kcmcddb test

INCLUDES = -I$(srcdir)/.. $(all_includes)

lib_LTLIBRARIES = libkcddb.la

libkcddb_la_SOURCES =\
cache.cpp\
cdinfo.cpp\
config.cpp\
client.cpp\
lookup.cpp\
cddblookup.cpp\
submit.cpp\
synccddblookup.cpp\
asynccddblookup.cpp

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
cddblookup.h\
submit.h\
synccddblookup.h\
asynccddblookup.h
