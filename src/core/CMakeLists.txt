add_library(core SHARED
    core.h
    core.c
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(core PRIVATE CORE_BUILDING)