cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None Debug Release." FORCE)
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/build" CACHE STRING "The install location." FORCE)
endif()

project(pizza VERSION 1.0 LANGUAGES CXX DESCRIPTION "Hashcode 2022 solution.")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/source SOURCE_EXE)
add_executable(${PROJECT_NAME} ${SOURCE_EXE})

target_include_directories(${PROJECT_NAME} PRIVATE include)

add_compile_options(
    -pipe
    -Wall
    -Wextra
    -Werror
    -Wpedantic
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options(
        -Ofast
        -march=native
    )
    add_link_options(
        -flto
    )
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(
        -Og
        -g3
    )
    if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        add_compile_options(
            -fsanitize=address,undefined
        )
        add_link_options(
            -fsanitize=address,undefined
        )
    endif()
else()
    message("Unsupported build type: '" ${CMAKE_BUILD_TYPE}')
    return()
endif()

install(DIRECTORY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/output)
