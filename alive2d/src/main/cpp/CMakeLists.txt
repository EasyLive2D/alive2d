cmake_minimum_required(VERSION 3.22.1)

project("alive2d")

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${CMAKE_PROJECT_NAME} SHARED
        live2d.cpp lappmodel.cpp)

target_include_directories(${CMAKE_PROJECT_NAME}
        PRIVATE
        ${PROJECT_ROOT}/Framework/src
        ${PROJECT_ROOT}/Main/src
)

add_subdirectory(Main)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        Main
        log)