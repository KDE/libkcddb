cmake_policy(SET CMP0028 NEW)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../libkcddb ${CMAKE_CURRENT_BINARY_DIR}/../libkcddb  )

add_subdirectory ( doc )

########### next target ###############

set(kcm_cddb_PART_SRCS cddbconfigwidget.cpp kcmcddb.cpp )
SET_SOURCE_FILES_PROPERTIES(cddbconfigwidget.cpp PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ui_cddbconfigwidget.h)


qt5_wrap_ui(kcm_cddb_PART_SRCS cddbconfigwidget.ui )

add_library(kcm_cddb ${kcm_cddb_PART_SRCS})



target_link_libraries(kcm_cddb  KF5::WidgetsAddons KF5::KIOCore KF5::KIOWidgets 
		      KF5::ConfigWidgets KF5::CoreAddons kcddb)

install(TARGETS kcm_cddb  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############

install( FILES libkcddb.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )


