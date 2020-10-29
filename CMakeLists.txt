cmake_minimum_required(VERSION 3.1)

project(ConsoleFW VERSION 0.0.1 LANGUAGES C)

option(BUILD_SHARED_LIBS    "Build shared libraries"        OFF)
option(CFW_INSTALL          "Generate installation target"  ON)

set(CFW_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

include(GNUInstallDirs)

# Use NCurses for console control
find_package(Curses REQUIRED)
list(APPEND cfw_INCLUDE_DIRS "${CURSES_INCLUDE_DIR}")
list(APPEND cfw_LIBRARIES "${CURSES_LIBRARIES}")

if (NOT CURSES_INCLUDE_DIR)
    message(FATAL_ERROR "nCurses headers not found; install ncurses development package")
endif()

# Add subdirectories
add_subdirectory(src)