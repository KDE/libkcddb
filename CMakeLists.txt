project(libkcddb)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)
include_directories(${KDE4_INCLUDES})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

if(MUSICBRAINZ4_FOUND)
    set(HAVE_MUSICBRAINZ4 1)
    include_directories(${MUSICBRAINZ4_INCLUDE_DIR})

    set(libmusicbrainz_SRCS ${libmusicbrainz_SRCS}
       musicbrainz/musicbrainzlookup.cpp
       musicbrainz/asyncmusicbrainzlookup.cpp)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")
else(MUSICBRAINZ4_FOUND)
    set(HAVE_MUSICBRAINZ4 0)
    set(libmusicbrainz_SRCS)
endif(MUSICBRAINZ4_FOUND)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-musicbrainz.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-musicbrainz.h )

add_subdirectory( test )
add_subdirectory( kcmcddb )
add_subdirectory( libkcddb )

install(EXPORT LibkcddbTargets DESTINATION ${LIB_INSTALL_DIR}/cmake/libkcddb/ FILE LibkcddbConfig.cmake NAMESPACE KDE::Libkcddb)
