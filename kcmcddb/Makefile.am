INCLUDES = -I$(srcdir)/../.. -I../.. $(all_includes)

kde_module_LTLIBRARIES = kcm_cddb.la

kcm_cddb_la_SOURCES = \
  cddbconfigwidgetbase.ui cddbconfigwidget.cpp kcmcddb.cpp

kcm_cddb_la_LDFLAGS = \
  $(all_libraries) -module -avoid-version -no-undefined

kcm_cddb_la_LIBADD = ../libkcddb.la $(LIB_KDEUI)

kcm_cddb_la_COMPILE_FIRST = ../configbase.h

METASOURCES = AUTO

KDE_ICON = cdrom_unmount

xdg_apps_DATA = libkcddb.desktop

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kcmcddb.pot

