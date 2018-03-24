include_directories(${PULSEAUDIO_INCLUDE_DIR} ${GLIB2_INCLUDE_DIR}     ${GCONF_INCLUDE_DIRS}
    ${GOBJECT_INCLUDE_DIRS})

set(cpp_SRCS
    card.cpp
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
    modulemanager.cpp
    gconfitem.cpp
    source.cpp
    sourceoutput.cpp
    stream.cpp
    volumeobject.cpp
    debug.cpp
    server.cpp
    streamrestore.cpp
    module.cpp
)


add_library(KF5PulseAudioQt SHARED ${cpp_SRCS})
target_link_libraries(KF5PulseAudioQt
    PUBLIC
        Qt5::Core
    PRIVATE
        ${PULSEAUDIO_LIBRARY}
        ${PULSEAUDIO_MAINLOOP_LIBRARY}
        ${GCONF_LDFLAGS}
        ${GOBJECT_LDFLAGS}
        ${CANBERRA_LIBRARIES}
)

set_target_properties(KF5PulseAudioQt PROPERTIES VERSION ${PULSEAUDIOQT_VERSION_STRING}
                                                   SOVERSION ${PULSEAUDIOQT_SOVERSION}
                                                   EXPORT_NAME PulseAudioQt
)
