cmake_minimum_required(VERSION 3.16)

project(
    CorporatePlanning
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4 /permissive-)
endif()

add_executable(corporate_planning
    src/main.cpp
    src/api/Cli.cpp
)

target_include_directories(corporate_planning
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(corporate_planning
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/bin
)

find_package(GTest QUIET)

if(GTest_FOUND)
    enable_testing()

    add_executable(corporate_planning_tests
        tests/test_main.cpp
    )

    target_link_libraries(corporate_planning_tests
        PRIVATE
            GTest::gtest
            GTest::gtest_main
    )

    add_test(
        NAME corporate_planning_tests
        COMMAND corporate_planning_tests
    )

    set_target_properties(corporate_planning_tests
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY
            ${CMAKE_BINARY_DIR}/bin
    )
else()
    message(STATUS "GTest not found - tests will be skipped")
endif()

include(GNUInstallDirs)

install(
    TARGETS corporate_planning
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
