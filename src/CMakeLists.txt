include_directories(${PULSEAUDIO_INCLUDE_DIR} ${GLIB2_INCLUDE_DIR})

set(cpp_SRCS
    card.cpp
    cardport.cpp
    client.cpp
    context.cpp
    device.cpp
    maps.cpp
    operation.cpp
    port.cpp
    profile.cpp
    pulseaudio.cpp
    pulseobject.cpp
    sink.cpp
    sinkinput.cpp
    source.cpp
    sourceoutput.cpp
    stream.cpp
    volumeobject.cpp
    server.cpp
    streamrestore.cpp
    module.cpp
)

ecm_qt_declare_logging_category(cpp_SRCS HEADER debug.h IDENTIFIER PULSEAUDIOQT CATEGORY_NAME org.kde.pulseaudio)

add_library(KF5PulseAudioQt SHARED ${cpp_SRCS})
target_link_libraries(KF5PulseAudioQt
    PUBLIC
        Qt5::Core
    PRIVATE
        Qt5::Gui
        Qt5::DBus
        ${PULSEAUDIO_LIBRARY}
        ${PULSEAUDIO_MAINLOOP_LIBRARY}
        ${CANBERRA_LIBRARIES}
)

target_include_directories(KF5PulseAudioQt INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KF5PulseAudioQt>" )

set_target_properties(KF5PulseAudioQt PROPERTIES VERSION ${PULSEAUDIOQT_VERSION_STRING}
                                                   SOVERSION ${PULSEAUDIOQT_SOVERSION}
                                                   EXPORT_NAME PulseAudioQt
)

ecm_generate_headers(PulseAudioQt_HEADERS
    HEADER_NAMES
        Card
        Client
        Context
        Device
        Module
        Port
        Profile
        PulseAudio
        PulseObject
        Server
        Sink
        SinkInput
        Source
        SourceOutput
        StreamRestore
        VolumeObject
    REQUIRED_HEADERS PulseAudioQt_HEADERS
)
generate_export_header(KF5PulseAudioQt BASE_NAME KF5PulseAudioQt)

install(TARGETS KF5PulseAudioQt EXPORT KF5PulseAudioQtTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${PulseAudioQt_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kf5pulseaudioqt_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KF5PulseAudioQt/PulseAudioQt COMPONENT Devel
)
