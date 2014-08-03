set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/../libkcddb  )

macro(kcddb_executable_tests)
	   foreach(_testname ${ARGN})
               add_executable(${_testname} ${_testname}.cpp)
               add_test( ${_testname} ${_testname} )
			   target_link_libraries(${_testname} Qt5::Test kcddb)
	   endforeach()
endmacro()

macro(kcddb_unit_tests)
	   foreach(_testname ${ARGN})
               add_executable(${_testname} ${_testname}.cpp)
               add_test( kcddb-${_testname} ${_testname} )
			   target_link_libraries(${_testname} Qt5::Test kcddb)
	   endforeach()
endmacro()

kcddb_unit_tests(
    asynchttplookuptest
    asynccddblookuptest
    synccddblookuptest
    synchttplookuptest
    utf8test
    musicbrainztest
    asyncmusicbrainztest
    cdinfotest
    cachetest
    musicbrainztest-severaldiscs
    musicbrainztest-fulldate
)

kcddb_executable_tests(
    syncsmtpsubmittest
    asyncsmtpsubmittest
    asynchttpsubmittest
    synchttpsubmittest
    sitestest
)
