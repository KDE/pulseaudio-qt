include(ECMAddTests)

ecm_add_test(
    ${CMAKE_SOURCE_DIR}/src/maps.h
    mapstest.cpp
    TEST_NAME mapstest
    LINK_LIBRARIES Qt5::Test KF5PulseAudioQt ${PULSEAUDIO_LIBRARY}
)
