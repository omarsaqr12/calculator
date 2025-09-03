cmake_minimum_required(VERSION 3.16)

project(QtAdvancedCalculator VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt components
find_package(Qt6 COMPONENTS Core Widgets QUIET)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets)
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

# Set Qt specific configurations
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Define the executable
set(SOURCES
    src/main.cpp
    src/calculator.cpp
)

set(HEADERS
    src/calculator.h
)

set(UI_FILES
    src/calculator.ui
)

# Create the executable
if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
    )
else()
    add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
    )
endif()

# Link Qt libraries
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets)
else()
    target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets)
endif()

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "QtAdvancedCalculator"
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Installation rules
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Platform-specific configurations
if(WIN32)
    # Windows specific settings
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
elseif(APPLE)
    # macOS specific settings
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
        MACOSX_BUNDLE_BUNDLE_NAME "Qt Advanced Calculator"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.qtadvancedcalculator"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    )
endif()

# Enable debugging in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE QT_DEBUG)
endif()

# Compiler-specific options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Print configuration summary
message(STATUS "Qt version: ${QT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
