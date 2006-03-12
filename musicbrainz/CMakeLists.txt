kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(srcdir)/.. $(all_includes)
#
#noinst_LTLIBRARIES = libmusicbrainz.la
#libmusicbrainz_la_SOURCES = musicbrainzlookup.cpp sha1.c
#libmusicbrainz_la_LIBADD = -lmusicbrainz
#
#METASOURCES = AUTO
#
