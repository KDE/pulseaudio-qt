set(painspector_SRCS
    main.cpp
)

qt5_add_resources(painspector_SRCS
    resources.qrc
)

add_executable(painspector ${painspector_SRCS})
target_link_libraries(painspector
    Qt5::Qml
    Qt5::Quick
    KF5PulseAudioQt
)
