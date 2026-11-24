add_library(asmimpl SHARED
    asmimpl.h
    core_contract.h
    contract_check.c
    
    $<$<PLATFORM_ID:Linux>:
        linux/detect_edges_params_offsets.inc
        linux/asmDetectEdges.s
        linux/asmDetectEdgesWorker.s
        linux/asmDetectEdgesSIMDWorker.s
    >

    $<$<PLATFORM_ID:Windows>:
        windows/detect_edges_params_offsets.inc
        windows/asmimpl.def
        windows/asmDetectEdges.asm
        windows/asmDetectEdgesWorker.asm
        windows/asmDetectEdgesSIMDWorker.asm
    >
)

target_include_directories(asmimpl PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    $<$<PLATFORM_ID:Linux>:${CMAKE_CURRENT_SOURCE_DIR}/linux>
    $<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/windows>
)
target_compile_definitions(asmimpl PRIVATE ASMIMPL_BUILDING)
target_link_libraries(asmimpl PUBLIC core)