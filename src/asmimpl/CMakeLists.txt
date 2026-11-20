add_library(asmimpl SHARED
    asmimpl.h
    
    $<$<PLATFORM_ID:Linux>:
        linux/asmDetectEdges.s
        linux/detect_edges_params_offsets.inc
        linux/asmDetectEdgesWorker.s
        linux/asmDetectEdgesSIMDWorker.s
    >

    $<$<PLATFORM_ID:Windows>:
    >
)

target_include_directories(asmimpl PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    $<$<PLATFORM_ID:Linux>:${CMAKE_CURRENT_SOURCE_DIR}/linux>
    $<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/windows>
)

target_link_libraries(asmimpl PUBLIC core)