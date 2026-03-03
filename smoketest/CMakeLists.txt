# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2026 Harald Sitter <sitter@kde.org>

include(ECMAddTests)

add_executable(pasmoketest main.cpp)
add_compile_definitions(-DUNDER_TEST)
target_link_libraries(pasmoketest Qt6::Test KF6PulseAudioQt PkgConfig::LIBPULSE)

add_test(
    NAME smoketest
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run.sh $<TARGET_FILE:pasmoketest>
)
