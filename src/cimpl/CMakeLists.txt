add_library(cimpl SHARED
    cimpl.h
    cimpl.c
)

target_include_directories(cimpl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(cimpl PUBLIC core)