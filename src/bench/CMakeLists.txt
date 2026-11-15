add_executable(bench
    bench.h
    bench.cpp
    main.cpp
)

target_compile_features(bench PRIVATE cxx_std_23)

target_link_libraries(bench PRIVATE core cimpl asmimpl Qt::Gui)

set_target_properties(bench PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release
)