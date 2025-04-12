cmake_minimum_required(VERSION 3.22.1)

project("alive2d")

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${CMAKE_PROJECT_NAME} SHARED
        Live2D.cpp Live2DModel.cpp)

target_include_directories(${CMAKE_PROJECT_NAME}
        PRIVATE
        ${PROJECT_ROOT}/Main/src
)

include(cmake/Core.cmake)
include(cmake/Framework.cmake)
include(cmake/Main.cmake)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        Main
        log)