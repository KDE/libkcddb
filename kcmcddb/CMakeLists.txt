add_subdirectory(doc)

ki18n_wrap_ui(ui_sources cddbconfigwidget.ui)

add_library(kcm_cddb MODULE
    cddbconfigwidget.cpp
    kcmcddb.cpp
    ${ui_sources})

set_source_files_properties(cddbconfigwidget.cpp PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ui_cddbconfigwidget.h)

target_link_libraries(kcm_cddb PRIVATE
    KF5::ConfigWidgets
    KF5::I18n
    KF5::KIOCore
    KF5::KIOWidgets
    KF5::WidgetsAddons
    KF5::Cddb
)

install(TARGETS kcm_cddb DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES libkcddb.desktop  DESTINATION  ${SERVICES_INSTALL_DIR})
