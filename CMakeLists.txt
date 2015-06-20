project(libkcddb)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)
include_directories(${KDE4_INCLUDES})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

macro_optional_find_package(MusicBrainz5)
macro_log_feature(MUSICBRAINZ5_FOUND "MusicBrainz5" "A library that provides access to metadata lookup on the MusicBrainz server" "http://www.musicbrainz.org" FALSE "" "Music metadata lookup for KDE multimedia applications through libkcddb. You need version 5.x of libmusicbrainz")

if(MUSICBRAINZ5_FOUND)
    set(HAVE_MUSICBRAINZ5 1)
    include_directories(${MUSICBRAINZ5_INCLUDE_DIRS})

    set(libmusicbrainz_SRCS ${libmusicbrainz_SRCS}
       musicbrainz/musicbrainzlookup.cpp
       musicbrainz/asyncmusicbrainzlookup.cpp)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")
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
