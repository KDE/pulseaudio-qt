set(painspector_SRCS
    main.cpp
)

qt_add_resources(painspector_SRCS
    resources.qrc
)

add_executable(painspector ${painspector_SRCS})
target_link_libraries(painspector
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::Quick
    KF${QT_MAJOR_VERSION}PulseAudioQt
)
