INCLUDES = -I$(top_srcdir) $(all_includes)

check_PROGRAMS = syncsmtpsubmittest asyncsmtpsubmittest asynchttplookuptest \
	asynccddblookuptest synccddblookuptest synchttplookuptest \
	utf8test utf8syncsubmittest utf8asyncsubmittest

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

utf8test_SOURCES = utf8test.cpp
utf8test_LDFLAGS = $(all_libraries)
utf8test_LDADD = $(top_builddir)/libkcddb/libkcddb.la

utf8syncsubmittest_SOURCES = utf8syncsubmittest.cpp
utf8syncsubmittest_LDFLAGS = $(all_libraries)
utf8syncsubmittest_LDADD = $(top_builddir)/libkcddb/libkcddb.la

utf8asyncsubmittest_SOURCES = utf8asyncsubmittest.cpp
utf8asyncsubmittest_LDFLAGS = $(all_libraries)
utf8asyncsubmittest_LDADD = $(top_builddir)/libkcddb/libkcddb.la

METASOURCES = AUTO

