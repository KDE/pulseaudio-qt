set(painspector_SRCS
    main.cpp
)

qt_add_resources(painspector_SRCS
    resources.qrc
)

add_executable(painspector ${painspector_SRCS})
target_link_libraries(painspector
    Qt6::Qml
    Qt6::Quick
    KF6PulseAudioQt
)
