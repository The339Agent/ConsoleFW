# Get source files
file(GLOB cfw_ROOT_SOURCES *.c)

# Add library output file
add_library(cfw ${cfw_ROOT_SOURCES})

# Set library properties
set_target_properties(cfw PROPERTIES
                      OUTPUT_NAME cfw
                      VERSION ${ConsoleFW_VERSION_MAJOR}.${ConsoleFW_VERSION_MINOR}
                      SOVERSION ${ConsoleFW_VERSION_MAJOR}
                      C_STANDARD 99
                      C_EXTENSIONS OFF)

# Include cfw headers
target_include_directories(cfw PUBLIC
                           "$<BUILD_INTERFACE:${CFW_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_include_directories(cfw PRIVATE 
                           "${CFW_SOURCE_DIR}/src"
                           "${ConsoleFW_BINARY_DIR}/src"
                           ${cfw_INCLUDE_DIRS})

# Set shared lib definitions
if (BUILD_SHARED_LIBS)
    target_compile_definitions(cfw PUBLIC   CFW_DLL)
    target_compile_definitions(cfw PRIVATE  _CFW_BUILD_DLL)
endif()

# Add libraries
target_link_libraries(cfw PRIVATE ${cfw_LIBRARIES})

# Enable some errors
target_compile_options(cfw PRIVATE "-Wall")

# Install ConsoleFW
if(CFW_INSTALL)
    install(TARGETS cfw
            EXPORT cfwTargets
            RUNTIME DESTINATION "bin"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()