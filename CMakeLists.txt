project(libkcddb)
cmake_minimum_required(VERSION 2.8.12)
find_package(ECM 1.0.0 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
cmake_policy(SET CMP0011 NEW)

set(QT_MIN_VERSION 5.4.0)
set(KF5_MIN_VERSION "5.16.0")

# Find Qt modules
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS 
    Core    # QCommandLineParser, QStringLiteral
    Widgets # QApplication
    Gui 
    Network
    Test
)

# Find KDE modules
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons      # KAboutData
    I18n            # KLocalizedString
    WidgetsAddons   # KMessageBox
    Solid
    Archive
    Config
    DocTools
    Codecs
    KIO
    Completion
    GuiAddons
    ItemModels
    ItemViews
    WindowSystem
)

include(ECMOptionalAddSubdirectory) 
include(ECMInstallIcons) 
include(ECMAddAppIcon)
include(ECMSetupVersion) 
include(ECMMarkNonGuiExecutable) 
include(ECMMarkAsTest)
include(ECMGenerateHeaders) 
include(GenerateExportHeader) 
include(CMakePackageConfigHelpers) 
include(FeatureSummary) 
include(WriteBasicConfigVersionFile) 
include(CheckFunctionExists) 
include(KDEInstallDirs) 
include(KDECMakeSettings) 
include(KDECompilerSettings) 
include_directories(${KDE5_INCLUDES})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

find_package(MusicBrainz5)
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

if(MUSICBRAINZ5_FOUND)
    set(HAVE_MUSICBRAINZ5 1)
    include_directories(${MUSICBRAINZ5_INCLUDE_DIRS})

    set(libmusicbrainz_SRCS ${libmusicbrainz_SRCS}
       musicbrainz/musicbrainzlookup.cpp
       musicbrainz/asyncmusicbrainzlookup.cpp)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
else(MUSICBRAINZ5_FOUND)
    set(HAVE_MUSICBRAINZ5 0)
    set(libmusicbrainz_SRCS)
endif(MUSICBRAINZ5_FOUND)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-musicbrainz.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-musicbrainz.h )

add_subdirectory( test )
add_subdirectory( kcmcddb )
add_subdirectory( libkcddb )

# Figure out the relative path from the installed Config.cmake file to the install prefix (which may be at
# runtime different from the chosen CMAKE_INSTALL_PREFIX if under Windows the package was installed anywhere)
# This relative path will be configured into LibkcddbConfig.cmake
file(RELATIVE_PATH relInstallDir ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/cmake/libkcddb ${CMAKE_INSTALL_PREFIX})

configure_file(LibkcddbConfig.cmake.in ${CMAKE_BINARY_DIR}/LibkcddbConfig.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/LibkcddbConfig.cmake DESTINATION ${LIB_INSTALL_DIR}/cmake/libkcddb)
install(EXPORT LibkcddbTargets DESTINATION ${LIB_INSTALL_DIR}/cmake/libkcddb/ FILE LibkcddbTargets.cmake NAMESPACE KDE::Libkcddb::)
