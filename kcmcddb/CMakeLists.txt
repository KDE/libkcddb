kde4_header()

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/.. ${KDE4_INCLUDES} )


########### next target ###############

set(kcm_cddb_PART_SRCS cddbconfigwidget.cpp kcmcddb.cpp )

kde4_automoc(${kcm_cddb_PART_SRCS})

kde4_add_ui_files(kcm_cddb_PART_SRCS cddbconfigwidget.ui )

kde4_add_plugin(kcm_cddb ${kcm_cddb_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kcm_cddb )

target_link_libraries(kcm_cddb  ${KDE4_KDEUI_LIBS} kcddb  )

install_targets(${LIB_INSTALL_DIR}/kde4 kcm_cddb )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES libkcddb.desktop )
install_files( ${KCONF_UPDATE_INSTALL_DIR} FILES kcmcddb-emailsettings.upd )

kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(srcdir)/../.. -I.. $(all_includes)
#
#kde_module_LTLIBRARIES = kcm_cddb.la
#
#kcm_cddb_la_SOURCES = \
#  cddbconfigwidgetbase.ui cddbconfigwidget.cpp kcmcddb.cpp
#
#kcm_cddb_la_LDFLAGS = \
#  $(all_libraries) -module -avoid-version -no-undefined
#
#kcm_cddb_la_LIBADD = ../libkcddb.la $(LIB_KDEUI)
#
#kcm_cddb_la_COMPILE_FIRST = ../configbase.h
#
#METASOURCES = AUTO
#
#xdg_apps_DATA = libkcddb.desktop
#
#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/kcmcddb.pot
#
#updatedir = $(kde_datadir)/kconf_update
#update_DATA = kcmcddb-emailsettings.upd
