add_library(asmimpl SHARED
    asmimpl.h
    gray.s
    edge.s
)

target_include_directories(asmimpl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})