add_library(KF5PulseAudioQt SHARED)

target_sources(KF5PulseAudioQt PRIVATE
    card.cpp
    cardport.cpp
    client.cpp
    context.cpp
    device.cpp
    maps.cpp
    operation.cpp
    port.cpp
    profile.cpp
    models.cpp
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
    indexedpulseobject.cpp
)

ecm_qt_declare_logging_category(KF5PulseAudioQt
    HEADER debug.h
    IDENTIFIER PULSEAUDIOQT
    CATEGORY_NAME org.kde.pulseaudio
)

target_link_libraries(KF5PulseAudioQt
    PUBLIC
        Qt5::Core
    PRIVATE
        Qt5::Gui
        Qt5::DBus
        PkgConfig::LIBPULSE
        PkgConfig::LIBPULSE_MAINLOOP
)

target_include_directories(KF5PulseAudioQt INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KF5PulseAudioQt>" )

set_target_properties(KF5PulseAudioQt PROPERTIES VERSION ${PULSEAUDIOQT_VERSION}
                                                   SOVERSION ${PULSEAUDIOQT_SOVERSION}
                                                   EXPORT_NAME PulseAudioQt
)

ecm_generate_headers(PulseAudioQt_HEADERS
    HEADER_NAMES
        Card
        CardPort
        Client
        Context
        Device
        IndexedPulseObject
        Module
        Port
        Profile
        Models
        PulseObject
        Server
        Sink
        SinkInput
        Source
        SourceOutput
        Stream
        StreamRestore
        VolumeObject
    REQUIRED_HEADERS PulseAudioQt_HEADERS
)
ecm_generate_export_header(KF5PulseAudioQt
    BASE_NAME PulseAudioQt
    GROUP_BASE_NAME KF
    VERSION ${PROJECT_VERSION}
)

install(TARGETS KF5PulseAudioQt EXPORT KF5PulseAudioQtTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${PulseAudioQt_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/pulseaudioqt_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KF5PulseAudioQt/PulseAudioQt COMPONENT Devel
)

if(BUILD_QCH)
    ecm_add_qch(
        KF5PulseAudioQt_QCH
        NAME PulseAudioQt
        BASE_NAME KF5PulseAudioQt
        VERSION ${PULSEAUDIOQT_VERSION}
        ORG_DOMAIN org.kde
        SOURCES
            ${PulseAudioQt_HEADERS}
        LINK_QCHS
            Qt5Gui_QCH
        BLANK_MACROS
            PulseAudioQt_EXPORT
            PulseAudioQt_DEPRECATED
            PulseAudioQt_DEPRECATED_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
