SUBDIRS = lookup

INCLUDES = -I$(top_srcdir) $(all_includes)

check_PROGRAMS = syncsmtpsubmittest

syncsmtpsubmittest_SOURCES = syncsmtpsubmittest.cpp
syncsmtpsubmittest_LDFLAGS = $(all_libraries)
syncsmtpsubmittest_LDADD = $(top_builddir)/libkcddb/libkcddb.la

METASOURCES = AUTO

