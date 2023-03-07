add_definitions(-DTRANSLATION_DOMAIN="libkcddb")

add_subdirectory(doc)

kcoreaddons_add_plugin(kcm_cddb INSTALL_NAMESPACE "plasma/kcms/systemsettings_qwidgets")
kcmutils_generate_desktop_file(kcm_cddb)

ki18n_wrap_ui(kcm_cddb cddbconfigwidget.ui)

target_sources(kcm_cddb PRIVATE
    cddbconfigwidget.cpp
    kcmcddb.cpp kcmcddb.h
)

set_source_files_properties(cddbconfigwidget.cpp PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ui_cddbconfigwidget.h)

target_link_libraries(kcm_cddb PRIVATE
    KF${KF_MAJOR_VERSION}::ConfigWidgets
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::KIOCore
    KF${KF_MAJOR_VERSION}::KIOWidgets
    KF${KF_MAJOR_VERSION}::WidgetsAddons
    KCddb
)
target_include_directories(kcm_cddb
    PRIVATE ${CMAKE_SOURCE_DIR} # for libkcddb/ prefixed includes of library headers
)
