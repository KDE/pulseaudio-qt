add_library(KF${QT_MAJOR_VERSION}PulseAudioQt SHARED)

target_sources(KF${QT_MAJOR_VERSION}PulseAudioQt PRIVATE
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

ecm_qt_declare_logging_category(KF${QT_MAJOR_VERSION}PulseAudioQt
    HEADER debug.h
    IDENTIFIER PULSEAUDIOQT
    CATEGORY_NAME org.kde.pulseaudio
)

target_link_libraries(KF${QT_MAJOR_VERSION}PulseAudioQt
    PUBLIC
        Qt${QT_MAJOR_VERSION}::Core
    PRIVATE
        Qt${QT_MAJOR_VERSION}::Gui
        Qt${QT_MAJOR_VERSION}::DBus
        PkgConfig::LIBPULSE
        PkgConfig::LIBPULSE_MAINLOOP
)

target_include_directories(KF${QT_MAJOR_VERSION}PulseAudioQt INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KF${QT_MAJOR_VERSION}PulseAudioQt>" )

set_target_properties(KF${QT_MAJOR_VERSION}PulseAudioQt PROPERTIES VERSION ${PULSEAUDIOQT_SOVERSION}.0.0
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
ecm_generate_export_header(KF${QT_MAJOR_VERSION}PulseAudioQt
    BASE_NAME PulseAudioQt
    GROUP_BASE_NAME KF
    VERSION ${PROJECT_VERSION}
)

ecm_generate_pkgconfig_file(BASE_NAME KF${QT_MAJOR_VERSION}PulseAudioQt
      INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KF${QT_MAJOR_VERSION}PulseAudioQt/
      DEPS "Qt${QT_MAJOR_VERSION}Core"
    INSTALL)

install(TARGETS KF${QT_MAJOR_VERSION}PulseAudioQt EXPORT PulseAudioQtTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${PulseAudioQt_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/pulseaudioqt_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KF${QT_MAJOR_VERSION}PulseAudioQt/PulseAudioQt COMPONENT Devel
)

if(BUILD_QCH)
    ecm_add_qch(
        KF${QT_MAJOR_VERSION}PulseAudioQt_QCH
        NAME PulseAudioQt
        BASE_NAME KF${QT_MAJOR_VERSION}PulseAudioQt
        VERSION ${PULSEAUDIOQT_VERSION}
        ORG_DOMAIN org.kde
        SOURCES
            ${PulseAudioQt_HEADERS}
        LINK_QCHS
            Qt${QT_MAJOR_VERSION}Gui_QCH
        BLANK_MACROS
            PulseAudioQt_EXPORT
            PulseAudioQt_DEPRECATED
            PulseAudioQt_DEPRECATED_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
