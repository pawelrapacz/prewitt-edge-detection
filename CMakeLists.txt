cmake_minimum_required(VERSION 3.31)
project(prewitt-edge-detection LANGUAGES CXX C ASM)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_subdirectory(src/cimpl)
add_subdirectory(src/asmimpl)
add_subdirectory(src/app)