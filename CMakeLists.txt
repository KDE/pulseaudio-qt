cmake_minimum_required(VERSION 3.0)

set(KF5_VERSION "5.45.0") # handled by release scripts
project(PulseAudioQt VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM 5.44.0  NO_MODULE)

set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMQtDeclareLoggingCategory)
include(ECMAddQch)

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(PROJECT VARIABLE_PREFIX PULSEAUDIOQT
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/pulseaudioqt_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5PulseAudioQtConfigVersion.cmake"
                        SOVERSION 1)

set(REQUIRED_QT_VERSION 5.7.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core)


# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5PulseAudioQt")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5PulseAudioQt_QCH
        FILE KF5PulseAudioQtTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5PulseAudioQtQchTargets.cmake\")")
endif()

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/KF5PulseAudioQtConfig.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/KF5PulseAudioQtConfigVersion.cmake"
               "${CMAKE_CURRENT_SOURCE_DIR}/KF5PulseAudioQtMacros.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel )

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5PulseAudioQtConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5PulseAudioQtConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)
        
include(FindPkgConfig)
pkg_check_modules(GCONF REQUIRED gconf-2.0)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    DocTools
    I18n
)
find_package(PulseAudio 5.0.0 REQUIRED)
find_package(Canberra REQUIRED)
find_package(GLIB2 REQUIRED)

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ecm_install_po_files_as_qm(po)
endif()

add_subdirectory(src)
#add_subdirectory(autotests)
#add_subdirectory(tests)

install(EXPORT KF5PulseAudioQtTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5PulseAudioQtTargets.cmake NAMESPACE KF5:: )

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/pulseaudioqt_version.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
