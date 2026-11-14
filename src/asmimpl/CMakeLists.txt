add_library(asmimpl SHARED
    asmimpl.h
    gray.s
    edge.s
    detect_edges_params_offsets.inc
    asmDetectEdgesWorker.s
)

target_include_directories(asmimpl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(asmimpl PUBLIC core)