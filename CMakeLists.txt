cmake_minimum_required(VERSION 3.5)

project(flappy-bird VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/font/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/font/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/world/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/world/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/icon/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/icon/)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
        Game.cpp
        Game.h
)

if(ANDROID)
    add_library(flappy-bird SHARED
        ${PROJECT_SOURCES}
    )
else()
    add_executable(flappy-bird
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(flappy-bird PRIVATE Qt5::Widgets)

set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.flappy-bird)
set_target_properties(flappy-bird PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS flappy-bird
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

