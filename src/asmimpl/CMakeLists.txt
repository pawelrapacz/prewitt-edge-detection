add_library(asmimpl SHARED
    asmimpl.h
    detect_edges_params_offsets.inc
    asmDetectEdges.s
    asmDetectEdgesWorker.s
    asmDetectEdgesSIMDWorker.s
    gray.s
)

target_include_directories(asmimpl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(asmimpl PUBLIC core)