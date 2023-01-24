add_definitions(-DTRANSLATION_DOMAIN="libkcddb")

add_subdirectory(doc)

add_library(kcm_cddb MODULE)

ki18n_wrap_ui(kcm_cddb cddbconfigwidget.ui)

target_sources(kcm_cddb PRIVATE
    cddbconfigwidget.cpp
    kcmcddb.cpp kcmcddb.h
)

set_source_files_properties(cddbconfigwidget.cpp PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ui_cddbconfigwidget.h)

target_link_libraries(kcm_cddb PRIVATE
    KF5::ConfigWidgets
    KF5::I18n
    KF5::KIOCore
    KF5::KIOWidgets
    KF5::WidgetsAddons
    KF5::Cddb
)
target_include_directories(kcm_cddb
    PRIVATE ${CMAKE_SOURCE_DIR} # for libkcddb/ prefixed includes of library headers
)

install(TARGETS kcm_cddb DESTINATION ${KDE_INSTALL_PLUGINDIR})

install(FILES libkcddb.desktop  DESTINATION  ${KDE_INSTALL_KSERVICESDIR})
