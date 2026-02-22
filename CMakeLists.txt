cmake_minimum_required(VERSION 4.1)

project(SudokuSolverGui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# ────────────────────────────────────────────────────────────────────────────────
# Qt Configuration
# ────────────────────────────────────────────────────────────────────────────────
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/ui")

# ────────────────────────────────────────────────────────────────────────────────
# Project Directories
# ────────────────────────────────────────────────────────────────────────────────
set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_DIR}/include)
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/src)
set(PROJECT_UI_DIR ${PROJECT_DIR}/ui)
set(PROJECT_RESOURCE_DIR ${PROJECT_DIR}/resources)
set(PROJECT_TEST_DIR ${PROJECT_DIR}/tests)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)


# ────────────────────────────────────────────────────────────────────────────────
# Source Files
# ────────────────────────────────────────────────────────────────────────────────
set(PROJECT_SOURCES
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/Sudoku.cpp
        ${PROJECT_SOURCE_DIR}/SudokuCell.cpp
        ${PROJECT_SOURCE_DIR}/SudokuGrid.cpp
        ${PROJECT_SOURCE_DIR}/MainWindow.cpp
)

set(PROJECT_HEADERS
        ${PROJECT_INCLUDE_DIR}/Sudoku.hpp
        ${PROJECT_INCLUDE_DIR}/SudokuCell.hpp
        ${PROJECT_INCLUDE_DIR}/SudokuGrid.hpp
        ${PROJECT_INCLUDE_DIR}/MainWindow.hpp
)

set(PROJECT_UI_FILES
        ${PROJECT_UI_DIR}/MainWindow.ui
)

include_directories(${PROJECT_INCLUDE_DIR})


# ────────────────────────────────────────────────────────────────────────────────
# Main Executable
# ────────────────────────────────────────────────────────────────────────────────
add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_UI_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Widgets
)

# ────────────────────────────────────────────────────────────────────────────────
# Testing (Google Test - Optional)
# ────────────────────────────────────────────────────────────────────────────────
enable_testing()

find_package(GTest QUIET)

if (GTest_FOUND)
    set(TEST_SOURCES
            ${PROJECT_TEST_DIR}/test_sudoku.cpp
    )

    add_executable(${PROJECT_NAME}_tests
            ${TEST_SOURCES}
            ${PROJECT_SOURCE_DIR}/Sudoku.cpp
    )

    target_include_directories(${PROJECT_NAME}_tests PRIVATE ${PROJECT_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::gtest GTest::gtest_main)

    add_test(NAME SudokuTests COMMAND ${PROJECT_NAME}_tests)
    message(STATUS "✓ Google Test framework found - unit tests enabled")
else ()
    message(WARNING "⚠ Google Test framework not found - unit tests disabled")
    message(WARNING "  Install with: brew install googletest (macOS)")
endif ()


# ────────────────────────────────────────────────────────────────────────────────
# Doxygen Documentation (Optional)
# ────────────────────────────────────────────────────────────────────────────────
find_package(Doxygen OPTIONAL_COMPONENTS dot QUIET)

if (DOXYGEN_FOUND)
    message(STATUS "✓ Doxygen found - documentation target available")
else ()
    message(WARNING "⚠ Doxygen not found - documentation generation disabled")
    message(WARNING "  Install with: brew install doxygen (macOS)")
endif ()



