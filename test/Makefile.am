INCLUDES = -I$(top_srcdir) -I$(top_builddir)/libkcddb $(all_includes)

check_PROGRAMS = syncsmtpsubmittest asyncsmtpsubmittest asynchttplookuptest \
	asynccddblookuptest synccddblookuptest synchttplookuptest \
	asynchttpsubmittest synchttpsubmittest sitestest utf8test

syncsmtpsubmittest_SOURCES = syncsmtpsubmittest.cpp
syncsmtpsubmittest_LDFLAGS = $(all_libraries)
syncsmtpsubmittest_LDADD = $(top_builddir)/libkcddb/libkcddb.la

asyncsmtpsubmittest_SOURCES = asyncsmtpsubmittest.cpp
asyncsmtpsubmittest_LDFLAGS = $(all_libraries)
asyncsmtpsubmittest_LDADD = $(top_builddir)/libkcddb/libkcddb.la

asynchttplookuptest_SOURCES = asynchttplookuptest.cpp
asynchttplookuptest_LDFLAGS = $(all_libraries)
asynchttplookuptest_LDADD = $(top_builddir)/libkcddb/libkcddb.la

asynccddblookuptest_SOURCES = asynccddblookuptest.cpp
asynccddblookuptest_LDFLAGS = $(all_libraries)
asynccddblookuptest_LDADD = $(top_builddir)/libkcddb/libkcddb.la

synchttplookuptest_SOURCES = synchttplookuptest.cpp
synchttplookuptest_LDFLAGS = $(all_libraries)
synchttplookuptest_LDADD = $(top_builddir)/libkcddb/libkcddb.la

synccddblookuptest_SOURCES = synccddblookuptest.cpp
synccddblookuptest_LDFLAGS = $(all_libraries)
synccddblookuptest_LDADD = $(top_builddir)/libkcddb/libkcddb.la

synchttpsubmittest_SOURCES = synchttpsubmittest.cpp
synchttpsubmittest_LDFLAGS = $(all_libraries)
synchttpsubmittest_LDADD = $(top_builddir)/libkcddb/libkcddb.la

asynchttpsubmittest_SOURCES = asynchttpsubmittest.cpp
asynchttpsubmittest_LDFLAGS = $(all_libraries)
asynchttpsubmittest_LDADD = $(top_builddir)/libkcddb/libkcddb.la

utf8test_SOURCES = utf8test.cpp
utf8test_LDFLAGS = $(all_libraries)
utf8test_LDADD = $(top_builddir)/libkcddb/libkcddb.la

sitestest_SOURCES = sitestest.cpp
sitestest_LDFLAGS = $(all_libraries)
sitestest_LDADD = $(top_builddir)/libkcddb/libkcddb.la

METASOURCES = AUTO

