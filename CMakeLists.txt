cmake_minimum_required(VERSION 3.16)

# ──────────────────────────────────────────────
# Project Definition
# ──────────────────────────────────────────────
project(
    CorporatePlanning
    VERSION 1.0.0
    DESCRIPTION "C++ implementation of the Firm Planning academic model with AI-driven optimization"
    LANGUAGES CXX
)

# ──────────────────────────────────────────────
# Global Build Options
# ──────────────────────────────────────────────
option(BUILD_TESTING "Build the unit tests" ON)
option(BUILD_DOCS "Generate Doxygen documentation" OFF)
option(CORPORATE_ENABLE_API "Enable the HTTP API layer (future)" OFF)

# ──────────────────────────────────────────────
# C++ Standard
# ──────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ──────────────────────────────────────────────
# Export compile_commands.json (for clangd/IDE)
# ──────────────────────────────────────────────
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ──────────────────────────────────────────────
# Compiler Warnings (professional dev experience)
# ──────────────────────────────────────────────
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4 /permissive-)
endif()

# ──────────────────────────────────────────────
# Library Target (the planning core)
# ──────────────────────────────────────────────
add_library(corporate_planning_core STATIC)

target_include_directories(corporate_planning_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Collect all implementation files under src/ (except api if disabled)
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/planning/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/optimization/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/simulation/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data/*.cpp
)

target_sources(corporate_planning_core PRIVATE ${CORE_SOURCES})

# ──────────────────────────────────────────────
# Executable Target (CLI entry point)
# ──────────────────────────────────────────────
add_executable(corporate_planning
    src/main.cpp
    src/api/Cli.cpp
)

target_link_libraries(corporate_planning PRIVATE corporate_planning_core)

# ──────────────────────────────────────────────
# Testing (Google Test / Catch2)
# ──────────────────────────────────────────────
if(BUILD_TESTING)
    enable_testing()

    find_package(GTest QUIET)

    if(GTest_FOUND)
        file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
        )

        add_executable(corporate_planning_tests ${TEST_SOURCES})
        target_link_libraries(corporate_planning_tests
            PRIVATE
                corporate_planning_core
                GTest::gtest
                GTest::gtest_main
        )
        include(GoogleTest)
        gtest_discover_tests(corporate_planning_tests)
    else()
        message(STATUS "GTest not found - tests will be skipped")
    endif()
endif()

# ──────────────────────────────────────────────
# Documentation (Doxygen)
# ──────────────────────────────────────────────
if(BUILD_DOCS)
    find_package(Doxygen QUIET)
    if(Doxygen_FOUND)
        doxygen_add_docs(
            docs
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            COMMENT "Generating API documentation"
        )
    endif()
endif()

# ────────────────────────────────────────────
# Output Directories (clean build artifacts)
# ──────────────────────────────────────────────
set_target_properties(corporate_planning corporate_planning_tests
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ──────────────────────────────────────────────
# Install Rules (optional)
# ──────────────────────────────────────────────
include(GNUInstallDirs)
install(TARGETS corporate_planning corporate_plan