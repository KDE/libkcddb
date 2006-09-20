
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/..  )


########### next target ###############

set(kcm_cddb_PART_SRCS cddbconfigwidget.cpp kcmcddb.cpp )
SET_SOURCE_FILES_PROPERTIES(cddbconfigwidget.cpp PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ui_cddbconfigwidget.h)

kde4_automoc(${kcm_cddb_PART_SRCS})

kde4_add_ui_files(kcm_cddb_PART_SRCS cddbconfigwidget.ui )

kde4_add_plugin(kcm_cddb ${kcm_cddb_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kcm_cddb )

target_link_libraries(kcm_cddb  ${KDE4_KDEUI_LIBS} kcddb  )

install(TARGETS kcm_cddb  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############

install( FILES libkcddb.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
install( FILES kcmcddb-emailsettings.upd  DESTINATION  ${KCONF_UPDATE_INSTALL_DIR} )


