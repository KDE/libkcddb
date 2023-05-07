function(add_unit_tests)
    foreach(test ${ARGN})
        add_executable(${test} ${test}.cpp)
        add_test(NAME ${test} COMMAND ${test})
        target_link_libraries(${test} Qt${QT_MAJOR_VERSION}::Test KCddb)
        target_include_directories(${test}
            PRIVATE ${CMAKE_SOURCE_DIR} # for libkcddb/ prefixed includes of library headers
        )
    endforeach()
endfunction()

add_unit_tests(
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
    asynchttpsubmittest
    synchttpsubmittest
    sitestest)
