add_library(asmimpl SHARED
    asmimpl.h
    
    $<$<AND:$<PLATFORM_ID:Linux>,$<C_COMPILER_ID:GNU,Clang>>:
        linux/asmDetectEdges.s
        linux/detect_edges_params_offsets.inc
        linux/asmDetectEdgesWorker.s
        linux/asmDetectEdgesSIMDWorker.s
        linux/gray.s
    >

    $<$<AND:$<PLATFORM_ID:Windows>,$<C_COMPILER_ID:MSVC>>:
    >
)


target_include_directories(asmimpl PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    $<$<PLATFORM_ID:Linux,Darwin>:${CMAKE_CURRENT_SOURCE_DIR}/linux>
    $<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/windows>
)

target_link_libraries(asmimpl PUBLIC core)