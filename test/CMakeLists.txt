
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/..  )

MACRO(KCDDB_EXECUTABLE_TESTS)
       FOREACH(_testname ${ARGN})
               kde4_automoc(${_testname}.cpp)
               kde4_add_test_executable(${_testname} ${_testname}.cpp)
               target_link_libraries(${_testname} ${KDE4_KDEUI_LIBS}
                                     ${QT_QTTEST_LIBRARY} kcddb)
       ENDFOREACH(_testname)
ENDMACRO(KCDDB_EXECUTABLE_TESTS)
MACRO(KCDDB_UNIT_TESTS)
       FOREACH(_testname ${ARGN})
               kde4_automoc(${_testname}.cpp)
               kde4_add_unit_test(${_testname} TESTNAME kcddb-${_testname} ${_testname}.cpp)
               target_link_libraries(${_testname} ${KDE4_KDEUI_LIBS}
                                     ${QT_QTTEST_LIBRARY} kcddb)
       ENDFOREACH(_testname)
ENDMACRO(KCDDB_UNIT_TESTS)

KCDDB_UNIT_TESTS(
    asynchttplookuptest
    asynccddblookuptest
    synccddblookuptest
    synchttplookuptest
    utf8test
    musicbrainztest
    asyncmusicbrainztest
    cdinfotest
)

KCDDB_EXECUTABLE_TESTS(
    syncsmtpsubmittest
    asyncsmtpsubmittest
    asynchttpsubmittest
    synchttpsubmittest
    sitestest
)

