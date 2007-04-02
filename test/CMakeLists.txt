
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/..  )

# Tests don't need to go into toplevel/bin, they are fine in the current dir.
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

MACRO(KCDDB_BUILD_TESTS)
       FOREACH(_testname ${ARGN})
               kde4_automoc(${_testname}.cpp)
               kde4_add_test(${_testname} ${_testname}.cpp)
               target_link_libraries(${_testname} ${KDE4_KDEUI_LIBS}
                                     ${QT_QTTEST_LIBRARY} kcddb)
       ENDFOREACH(_testname)
ENDMACRO(KCDDB_BUILD_TESTS)
MACRO(KCDDB_EXECUTE_TESTS)
       FOREACH(_testname ${ARGN})
               add_test(kcddb-${_testname} ${EXECUTABLE_OUTPUT_PATH}/${_testname})
       ENDFOREACH(_testname)
ENDMACRO(KCDDB_EXECUTE_TESTS)

set(kcddb_unittests
    asynchttplookuptest
    asynccddblookuptest
    synccddblookuptest
    synchttplookuptest
    utf8test
    musicbrainztest
    asyncmusicbrainztest
    cdinfotest
)

KCDDB_BUILD_TESTS(
    ${kcddb_unittests}
    syncsmtpsubmittest
    asyncsmtpsubmittest
    asynchttpsubmittest
    synchttpsubmittest
    sitestest
)

KCDDB_EXECUTE_TESTS(
    ${kcddb_unittests})
