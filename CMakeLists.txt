cmake_minimum_required(VERSION 3.16)

set(PROJECT_VERSION 1.6.1)

project(PulseAudioQt VERSION ${PROJECT_VERSION})

include(FeatureSummary)
find_package(ECM 6.0.0  NO_MODULE)
set(KDE_COMPILERSETTINGS_LEVEL "6.0")
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://kde.org/products/frameworks/")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMGenerateExportHeader)
include(ECMQtDeclareLoggingCategory)
include(ECMAddQch)
include(KDEClangFormat)
include(ECMGeneratePkgConfigFile)

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(KDEGitCommitHooks)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(PROJECT VARIABLE_PREFIX PULSEAUDIOQT
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/pulseaudioqt_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF6PulseAudioQtConfigVersion.cmake"
                        SOVERSION 5)

set(REQUIRED_QT_VERSION 6.7.0)
find_package(Qt6 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Gui DBus)

find_package(Qt6Test ${REQUIRED_QT_VERSION} CONFIG)
set_package_properties(Qt6Test PROPERTIES DESCRIPTION "Autotests for PulseAudioQt" TYPE OPTIONAL)

find_package(Qt6Qml ${REQUIRED_QT_VERSION} CONFIG)
set_package_properties(Qt6Qml PROPERTIES DESCRIPTION "Tests for PulseAudioQt" TYPE OPTIONAL)

find_package(Qt6Quick ${REQUIRED_QT_VERSION} CONFIG)
set_package_properties(Qt6Quick PROPERTIES DESCRIPTION "Tests for PulseAudioQt" TYPE OPTIONAL)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF6PulseAudioQt")


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/KF6PulseAudioQtConfig.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/KF6PulseAudioQtConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel )

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF6PulseAudioQtConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF6PulseAudioQtConfig.cmake"
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
    if(Qt6Test_FOUND)
        add_subdirectory(autotests)
    endif()
endif()

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF6PulseAudioQt_QCH
        FILE KF6PulseAudioQtQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF6PulseAudioQtQchTargets.cmake\")")
endif()

install(EXPORT PulseAudioQtTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF6PulseAudioQtTargets.cmake NAMESPACE KF6:: )

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/pulseaudioqt_version.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF} COMPONENT Devel )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

# add clang-format target for all our real source files
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
