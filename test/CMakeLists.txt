kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(syncsmtpsubmittest_SRCS syncsmtpsubmittest.cpp )

kde4_automoc(${syncsmtpsubmittest_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(syncsmtpsubmittest ${syncsmtpsubmittest_SRCS})

target_link_libraries(syncsmtpsubmittest  ${QT_AND_KDECORE_LIBS} kcddb )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(asyncsmtpsubmittest_SRCS asyncsmtpsubmittest.cpp )

kde4_automoc(${asyncsmtpsubmittest_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(asyncsmtpsubmittest ${asyncsmtpsubmittest_SRCS})

target_link_libraries(asyncsmtpsubmittest  ${QT_AND_KDECORE_LIBS} kcddb )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(asynchttplookuptest_SRCS asynchttplookuptest.cpp )

kde4_automoc(${asynchttplookuptest_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(asynchttplookuptest ${asynchttplookuptest_SRCS})

target_link_libraries(asynchttplookuptest  ${QT_AND_KDECORE_LIBS} kcddb QtTest_debug )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(asynccddblookuptest_SRCS asynccddblookuptest.cpp )

kde4_automoc(${asynccddblookuptest_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(asynccddblookuptest ${asynccddblookuptest_SRCS})

target_link_libraries(asynccddblookuptest  ${QT_AND_KDECORE_LIBS} kcddb )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(synccddblookuptest_SRCS synccddblookuptest.cpp )

kde4_automoc(${synccddblookuptest_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(synccddblookuptest ${synccddblookuptest_SRCS})

target_link_libraries(synccddblookuptest  ${QT_AND_KDECORE_LIBS} kcddb QtTest_debug )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(synchttplookuptest_SRCS synchttplookuptest.cpp )

kde4_automoc(${synchttplookuptest_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(synchttplookuptest ${synchttplookuptest_SRCS})

target_link_libraries(synchttplookuptest  ${QT_AND_KDECORE_LIBS} kcddb )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(asynchttpsubmittest_SRCS asynchttpsubmittest.cpp )

kde4_automoc(${asynchttpsubmittest_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(asynchttpsubmittest ${asynchttpsubmittest_SRCS})

target_link_libraries(asynchttpsubmittest  ${QT_AND_KDECORE_LIBS} kcddb )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(synchttpsubmittest_SRCS synchttpsubmittest.cpp )

kde4_automoc(${synchttpsubmittest_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(synchttpsubmittest ${synchttpsubmittest_SRCS})

target_link_libraries(synchttpsubmittest  ${QT_AND_KDECORE_LIBS} kcddb )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(sitestest_SRCS sitestest.cpp )

kde4_automoc(${sitestest_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(sitestest ${sitestest_SRCS})

target_link_libraries(sitestest  ${QT_AND_KDECORE_LIBS} kcddb )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(utf8test_SRCS utf8test.cpp )

kde4_automoc(${utf8test_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(utf8test ${utf8test_SRCS})

target_link_libraries(utf8test  ${QT_AND_KDECORE_LIBS} kcddb )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(musicbrainztest_SRCS musicbrainztest.cpp )

kde4_automoc(${musicbrainztest_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(musicbrainztest ${musicbrainztest_SRCS})

target_link_libraries(musicbrainztest  ${QT_AND_KDECORE_LIBS} kcddb QtTest_debug )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(cdinfotest_SRCS cdinfotest.cpp )

kde4_automoc(${cdinfotest_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(cdinfotest ${cdinfotest_SRCS})

target_link_libraries(cdinfotest  ${QT_AND_KDECORE_LIBS} kcddb QtTest_debug )

endif(KDE4_BUILD_TESTS)

########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir) -I$(top_builddir)/libkcddb $(all_includes)
#
#check_PROGRAMS = syncsmtpsubmittest asyncsmtpsubmittest asynchttplookuptest \
#	asynccddblookuptest synccddblookuptest synchttplookuptest \
#	asynchttpsubmittest synchttpsubmittest sitestest utf8test \
#	musicbrainztest cdinfotest
#
#AM_LDFLAGS = $(QT_LDFLAGS) $(X_LDFLAGS) $(KDE_RPATH)
#
#syncsmtpsubmittest_SOURCES = syncsmtpsubmittest.cpp
#syncsmtpsubmittest_LDFLAGS = $(all_libraries)
#syncsmtpsubmittest_LDADD = $(top_builddir)/libkcddb/libkcddb.la
#
#asyncsmtpsubmittest_SOURCES = asyncsmtpsubmittest.cpp
#asyncsmtpsubmittest_LDFLAGS = $(all_libraries)
#asyncsmtpsubmittest_LDADD = $(top_builddir)/libkcddb/libkcddb.la
#
#asynchttplookuptest_SOURCES = asynchttplookuptest.cpp
#asynchttplookuptest_LDFLAGS = $(all_libraries)
#asynchttplookuptest_LDADD = $(top_builddir)/libkcddb/libkcddb.la -lQtTest_debug
#
#asynccddblookuptest_SOURCES = asynccddblookuptest.cpp
#asynccddblookuptest_LDFLAGS = $(all_libraries)
#asynccddblookuptest_LDADD = $(top_builddir)/libkcddb/libkcddb.la
#
#synchttplookuptest_SOURCES = synchttplookuptest.cpp
#synchttplookuptest_LDFLAGS = $(all_libraries)
#synchttplookuptest_LDADD = $(top_builddir)/libkcddb/libkcddb.la
#
#synccddblookuptest_SOURCES = synccddblookuptest.cpp
#synccddblookuptest_LDFLAGS = $(all_libraries)
#synccddblookuptest_LDADD = $(top_builddir)/libkcddb/libkcddb.la -lQtTest_debug
#
#synchttpsubmittest_SOURCES = synchttpsubmittest.cpp
#synchttpsubmittest_LDFLAGS = $(all_libraries)
#synchttpsubmittest_LDADD = $(top_builddir)/libkcddb/libkcddb.la
#
#asynchttpsubmittest_SOURCES = asynchttpsubmittest.cpp
#asynchttpsubmittest_LDFLAGS = $(all_libraries)
#asynchttpsubmittest_LDADD = $(top_builddir)/libkcddb/libkcddb.la
#
#musicbrainztest_SOURCES = musicbrainztest.cpp
#musicbrainztest_LDFLAGS = $(all_libraries)
#musicbrainztest_LDADD = $(top_builddir)/libkcddb/libkcddb.la -lQtTest_debug
#
#utf8test_SOURCES = utf8test.cpp
#utf8test_LDFLAGS = $(all_libraries)
#utf8test_LDADD = $(top_builddir)/libkcddb/libkcddb.la
#
#sitestest_SOURCES = sitestest.cpp
#sitestest_LDFLAGS = $(all_libraries)
#sitestest_LDADD = $(top_builddir)/libkcddb/libkcddb.la
#
#cdinfotest_SOURCES = cdinfotest.cpp
#cdinfotest_LDFLAGS = $(all_libraries)
#cdinfotest_LDADD = $(top_builddir)/libkcddb/libkcddb.la -lQtTest_debug
#
#TESTS = synccddblookuptest cdinfotest asynchttplookuptest musicbrainztest
#
#METASOURCES = AUTO
#
