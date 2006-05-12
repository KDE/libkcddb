
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/..  )


if(KDE4_BUILD_TESTS)

    set(syncsmtpsubmittest_SRCS syncsmtpsubmittest.cpp )
    kde4_automoc(${syncsmtpsubmittest_SRCS})
    kde4_add_executable(syncsmtpsubmittest RUN_UNINSTALLED ${syncsmtpsubmittest_SRCS})
    target_link_libraries(syncsmtpsubmittest  ${KDE4_KDECORE_LIBS} kcddb )

    set(asyncsmtpsubmittest_SRCS asyncsmtpsubmittest.cpp )
    kde4_automoc(${asyncsmtpsubmittest_SRCS})
    kde4_add_executable(asyncsmtpsubmittest RUN_UNINSTALLED ${asyncsmtpsubmittest_SRCS})
    target_link_libraries(asyncsmtpsubmittest  ${KDE4_KDECORE_LIBS} kcddb )

    set(asynchttplookuptest_SRCS asynchttplookuptest.cpp )
    kde4_automoc(${asynchttplookuptest_SRCS})
    kde4_add_executable(asynchttplookuptest RUN_UNINSTALLED ${asynchttplookuptest_SRCS})
    target_link_libraries(asynchttplookuptest  ${KDE4_KDECORE_LIBS} kcddb QtTest_debug )

    set(asynccddblookuptest_SRCS asynccddblookuptest.cpp )
    kde4_automoc(${asynccddblookuptest_SRCS})
    kde4_add_executable(asynccddblookuptest RUN_UNINSTALLED ${asynccddblookuptest_SRCS})
    target_link_libraries(asynccddblookuptest  ${KDE4_KDECORE_LIBS} kcddb QtTest_debug )

    set(synccddblookuptest_SRCS synccddblookuptest.cpp )
    kde4_automoc(${synccddblookuptest_SRCS})
    kde4_add_executable(synccddblookuptest RUN_UNINSTALLED ${synccddblookuptest_SRCS})
    target_link_libraries(synccddblookuptest  ${KDE4_KDECORE_LIBS} kcddb QtTest_debug )

    set(synchttplookuptest_SRCS synchttplookuptest.cpp )
    kde4_automoc(${synchttplookuptest_SRCS})
    kde4_add_executable(synchttplookuptest RUN_UNINSTALLED ${synchttplookuptest_SRCS})
    target_link_libraries(synchttplookuptest  ${KDE4_KDECORE_LIBS} kcddb QtTest_debug )

    set(asynchttpsubmittest_SRCS asynchttpsubmittest.cpp )
    kde4_automoc(${asynchttpsubmittest_SRCS})
    kde4_add_executable(asynchttpsubmittest RUN_UNINSTALLED ${asynchttpsubmittest_SRCS})
    target_link_libraries(asynchttpsubmittest  ${KDE4_KDECORE_LIBS} kcddb )

    set(synchttpsubmittest_SRCS synchttpsubmittest.cpp )
    kde4_automoc(${synchttpsubmittest_SRCS})
    kde4_add_executable(synchttpsubmittest RUN_UNINSTALLED ${synchttpsubmittest_SRCS})
    target_link_libraries(synchttpsubmittest  ${KDE4_KDECORE_LIBS} kcddb )

    set(sitestest_SRCS sitestest.cpp )
    kde4_automoc(${sitestest_SRCS})
    kde4_add_executable(sitestest RUN_UNINSTALLED ${sitestest_SRCS})
    target_link_libraries(sitestest  ${KDE4_KDECORE_LIBS} kcddb )

    set(utf8test_SRCS utf8test.cpp )
    kde4_automoc(${utf8test_SRCS})
    kde4_add_executable(utf8test RUN_UNINSTALLED ${utf8test_SRCS})
    target_link_libraries(utf8test  ${KDE4_KDECORE_LIBS} kcddb QtTest_debug )

    set(musicbrainztest_SRCS musicbrainztest.cpp )
    kde4_automoc(${musicbrainztest_SRCS})
    kde4_add_executable(musicbrainztest RUN_UNINSTALLED ${musicbrainztest_SRCS})
    target_link_libraries(musicbrainztest  ${KDE4_KDECORE_LIBS} kcddb QtTest_debug )

    set(cdinfotest_SRCS cdinfotest.cpp )
    kde4_automoc(${cdinfotest_SRCS})
    kde4_add_executable(cdinfotest RUN_UNINSTALLED ${cdinfotest_SRCS})
    target_link_libraries(cdinfotest  ${KDE4_KDECORE_LIBS} kcddb QtTest_debug )

endif(KDE4_BUILD_TESTS)


