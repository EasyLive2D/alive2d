cmake_minimum_required(VERSION 3.22.1)

project("alive2d")

include(Live2D/cmake/Live2D.cmake)

add_library(${CMAKE_PROJECT_NAME} SHARED
        Live2D.cpp Live2DModel.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        Live2D::Main
        log)