SUBDIRS = . kcmcddb test

INCLUDES = -I$(srcdir)/.. $(all_includes)

lib_LTLIBRARIES = libkcddb.la

libkcddb_la_SOURCES =\
asynccddblookup.cpp\
asyncclient.cpp\
asynchttplookup.cpp\
asynclookup.cpp\
cache.cpp\
cdinfo.cpp\
config.cpp\
client.cpp\
defines.cpp\
lookup.cpp\
synclookup.cpp\
submit.cpp\
synccddblookup.cpp\
trackinfolist.cpp

libkcddb_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -module
libkcddb_la_LIBADD  = $(LIB_KDECORE) $(LIB_KIO)

METASOURCES = AUTO

kcddbincludedir = $(includedir)/libkcddb
kcddbinclude_HEADERS =\
asynccddblookup.h\
asyncclient.h\
asynchttplookup.h\
asynclookup.h\
cache.h\
cdinfo.h\
client.h\
config.h\
defines.h\
lookup.h\
submit.h\
synccddblookup.h\
synclookup.h\
trackinfolist.h
