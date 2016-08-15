function(add_unit_tests)
    foreach(test ${ARGN})
        add_executable(${test} ${test}.cpp)
        add_test( ${test} ${test} )
        target_link_libraries(${test} Qt5::Test KF5::Cddb)
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
    syncsmtpsubmittest
    asyncsmtpsubmittest
    asynchttpsubmittest
    synchttpsubmittest
    sitestest)
