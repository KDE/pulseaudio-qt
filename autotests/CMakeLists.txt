include(ECMAddTests)

add_definitions(-DUNDER_TEST)
ecm_add_test(
    ${CMAKE_SOURCE_DIR}/src/maps.h
    ${CMAKE_SOURCE_DIR}/src/module.cpp
    ${CMAKE_BINARY_DIR}/src/debug.cpp
    mapstest.cpp
    TEST_NAME mapstest
    LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test KF${QT_MAJOR_VERSION}PulseAudioQt PkgConfig::LIBPULSE
)
