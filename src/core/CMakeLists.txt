add_library(core SHARED
    core.h
    core.c
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})