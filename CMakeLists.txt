cmake_minimum_required(VERSION 3.31)
project(prewitt-edge-detection LANGUAGES CXX C)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    enable_language(ASM)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    enable_language(ASM_MASM)
else()
    message(FATAL_ERROR "Platform ${CMAKE_SYSTEM_NAME} is not supported!\nSupported platforms: Linux, Windows.")
endif()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(VISIBILITY_INLINES_HIDDEN ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

add_subdirectory(src/core)
add_subdirectory(src/cimpl)
add_subdirectory(src/asmimpl)
add_subdirectory(src/app)
add_subdirectory(src/bench)