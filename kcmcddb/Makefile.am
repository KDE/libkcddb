INCLUDES = -I$(srcdir)/../.. $(all_includes)

kde_module_LTLIBRARIES = libkcm_cddb_config.la

libkcm_cddb_config_la_SOURCES = \
  cddbconfigwidgetbase.ui cddbconfigwidget.cpp kcmcddb.cpp

libkcm_cddb_config_la_LDFLAGS = \
  $(all_libraries) -module -avoid-version -no-undefined

libkcm_cddb_config_la_LIBADD = ../libkcddb.la $(LIB_KDEUI)

METASOURCES = AUTO

KDE_ICON = cdrom_unmount

data_DATA = cddb.desktop
datadir = $(kde_appsdir)/Settings/Sound

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kcmcddb.pot

