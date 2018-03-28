include(ECMAddTests)

ecm_add_tests(
    mapstest.cpp

    LINK_LIBRARIES Qt5::Test KF5PulseAudioQt
)
