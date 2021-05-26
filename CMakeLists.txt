cmake_minimum_required(VERSION 3.16)

set(PROJECT_VERSION 1.3.0)

project(PulseAudioQt VERSION ${PROJECT_VERSION})

include(FeatureSummary)
find_package(ECM 5.80.0  NO_MODULE)

set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://kde.org/products/frameworks/")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMGenerateExportHeader)
include(ECMQtDeclareLoggingCategory)
include(ECMAddQch)
include(KDEClangFormat)

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
# include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(PROJECT VARIABLE_PREFIX PULSEAUDIOQT
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/pulseaudioqt_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5PulseAudioQtConfigVersion.cmake"
                        SOVERSION 3)

set(REQUIRED_QT_VERSION 5.15.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Gui DBus)

find_package(Qt5Test ${REQUIRED_QT_VERSION} CONFIG)
set_package_properties(Qt5Test PROPERTIES DESCRIPTION "Autotests for PulseAudioQt" TYPE OPTIONAL)

find_package(Qt5Qml ${REQUIRED_QT_VERSION} CONFIG)
set_package_properties(Qt5Qml PROPERTIES DESCRIPTION "Tests for PulseAudioQt" TYPE OPTIONAL)

find_package(Qt5Quick ${REQUIRED_QT_VERSION} CONFIG)
set_package_properties(Qt5Quick PROPERTIES DESCRIPTION "Tests for PulseAudioQt" TYPE OPTIONAL)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5PulseAudioQt")


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/KF5PulseAudioQtConfig.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/KF5PulseAudioQtConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel )

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5PulseAudioQtConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5PulseAudioQtConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBPULSE libpulse REQUIRED IMPORTED_TARGET)
pkg_check_modules(LIBPULSE_MAINLOOP libpulse-mainloop-glib REQUIRED IMPORTED_TARGET)

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ecm_install_po_files_as_qm(po)
endif()

add_subdirectory(src)

if (BUILD_TESTING)
    add_subdirectory(tests)
    if(Qt5Test_FOUND)
        add_subdirectory(autotests)
    endif()
endif()

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5PulseAudioQt_QCH
        FILE KF5PulseAudioQtQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5PulseAudioQtQchTargets.cmake\")")
endif()

install(EXPORT KF5PulseAudioQtTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5PulseAudioQtTargets.cmake NAMESPACE KF5:: )

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/pulseaudioqt_version.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

# add clang-format target for all our real source files
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
