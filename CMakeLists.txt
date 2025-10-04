cmake_minimum_required(VERSION 3.5)
project(CMakeHW)

#设置默认构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose build type" FORCE)
endif()

#项目开关
option(USE_MYMATH "Use custom math library" ON)
option(USE_IMAGE_PROC "Enable image processing" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" ON)
option(BUILD_TESTS "Build test cases" ON)

#配置头文件
configure_file(config.h.in config.h)

#包含目录
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#MathFunctions库
if(USE_MYMATH)
    if(BUILD_SHARED_LIBS)
        add_library(MathFunctions SHARED math/MathFunctions.cpp)
        target_include_directories(MathFunctions
            PUBLIC
                ${CMAKE_CURRENT_SOURCE_DIR}/math
        )
        set_target_properties(MathFunctions PROPERTIES OUTPUT_NAME "MathFunctions")
        message(STATUS "Building MathFunctions as SHARED library")
    endif()

    if(BUILD_STATIC_LIBS)
        add_library(MathFunctions_static STATIC math/MathFunctions.cpp)
        target_include_directories(MathFunctions_static
            PUBLIC
                ${CMAKE_CURRENT_SOURCE_DIR}/math
        )
        set_target_properties(MathFunctions_static PROPERTIES OUTPUT_NAME "MathFunctions")
        message(STATUS "Building MathFunctions as STATIC library")
    endif()
endif()

#OpenCV依赖
if(USE_IMAGE_PROC)
    find_package(OpenCV REQUIRED)
    if(OpenCV_FOUND)
        message(STATUS "OpenCV found: ${OpenCV_VERSION}")
        add_library(ImageProcessor image_proc/ImageProcessor.cpp)
        target_link_libraries(ImageProcessor ${OpenCV_LIBS})
    else()
        message(FATAL_ERROR "OpenCV not found but USE_IMAGE_PROC is ON")
    endif()
endif()

#main--executable
add_executable(CMakeHW main.cpp)
target_include_directories(CMakeHW
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/math
        ${CMAKE_CURRENT_SOURCE_DIR}/image_proc
)

#链接库
if(USE_MYMATH)
    if(BUILD_SHARED_LIBS)
        target_link_libraries(CMakeHW MathFunctions)
    elseif(BUILD_STATIC_LIBS)
        target_link_libraries(CMakeHW MathFunctions_static)
    endif()
endif()

if(USE_IMAGE_PROC AND OpenCV_FOUND)
    target_link_libraries(CMakeHW ImageProcessor)
endif()

#测试power方法
if(BUILD_TESTS)
    enable_testing()

    #testing executable
    add_executable(test_power test/test_power.cpp)

    target_include_directories(test_power
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/math
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

    #链接MathFunctions库
    if(USE_MYMATH)
        if(BUILD_SHARED_LIBS)
            target_link_libraries(test_power MathFunctions)
        elseif(BUILD_STATIC_LIBS)
            target_link_libraries(test_power MathFunctions_static)
        endif()
    endif()

    #测试
    add_test(NAME PowerFunctionTest COMMAND test_power)
    message(STATUS "Tests enabled -- test_power will be built")
endif()

#CMake Debug
option(ENABLE_CMAKE_DEBUG "Enable Cmake debug output" OFF)
if(ENABLE_CMAKE_DEBUG)
    message(STATUS "---CMake Debug Information---")
    message(STATUS "Project: ${PROJECT_NAME}")
    message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
    message(STATUS "Source Dir: ${CMAKE_SOURCE_DIR}")
    message(STATUS "Binary Dir: ${CMAKE_BINARY_DIR}")
    message(STATUS "USE_MYMATH: ${USE_MYMATH}")
    message(STATUS "USE_IMAGE_PROC: ${USE_IMAGE_PROC}")
    message(STATUS "BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
    message(STATUS "BUILD_STATIC_LIBS: ${BUILD_STATIC_LIBS}")
    message(STATUS "BUILD_TESTS: ${BUILD_TESTS}")
    if(OpenCV_FOUND)
        message(STATUS "OpenCV Version: ${OpenCV_VERSION}")
    endif()
endif()

