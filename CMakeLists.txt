cmake_minimum_required(VERSION 3.16)
project(QtAdvancedCalculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    set(QT_MAJOR 6)
else()
    find_package(Qt5 5.15 REQUIRED COMPONENTS Widgets)
    set(QT_MAJOR 5)
endif()

set(CALCULATOR_SOURCES
    src/calculator.cpp
    src/calculator.h
    src/calculator.ui
)

add_executable(QtAdvancedCalculator src/main.cpp ${CALCULATOR_SOURCES})
target_link_libraries(QtAdvancedCalculator PRIVATE Qt${QT_MAJOR}::Widgets)
set_target_properties(QtAdvancedCalculator PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

include(GNUInstallDirs)
install(TARGETS QtAdvancedCalculator
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(CTest)
if(BUILD_TESTING)
    add_executable(calculator_smoke tests/calculator_smoke.cpp ${CALCULATOR_SOURCES})
    target_link_libraries(calculator_smoke PRIVATE Qt${QT_MAJOR}::Widgets)
    add_test(NAME calculator_arithmetic_smoke COMMAND calculator_smoke)
    set_tests_properties(calculator_arithmetic_smoke PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
    )
endif()
