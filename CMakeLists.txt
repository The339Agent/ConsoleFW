cmake_minimum_required(VERSION 3.1)

project(ConsoleFW VERSION 0.0.1 LANGUAGES C)

option(BUILD_SHARED_LIBS    "Build shared libraries"        OFF)
option(CFW_INSTALL          "Generate installation target"  ON)

set(CFW_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

include(GNUInstallDirs)

# Select backend API
if (UNIX AND NOT APPLE)
    set(_CFW_USE_CURSES TRUE)
    message(STATUS "Using Curses for console manipulation")
else()
    message(FATAL_ERROR "No supported platform detected")
endif()

# Use Curses for console manipulation
if (_CFW_USE_CURSES)
    find_package(Curses REQUIRED)

    list(APPEND cfw_INCLUDE_DIRS "${CURSES_INCLUDE_DIRS}")
    list(APPEND cfw_LIBRARIES "${CURSES_LIBRARIES}")

    if (NOT ${CURSES_FOUND})
        message(FATAL_ERROR "Curses headers not found; install ncurses development package")
    endif()
endif()

# Add subdirectories
add_subdirectory(src)