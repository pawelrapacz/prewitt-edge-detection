add_executable(bench
    bench.h
    bench.cpp
    main.cpp
)

target_compile_features(bench PRIVATE cxx_std_23)

target_link_libraries(bench PRIVATE core cimpl asmimpl Qt::Gui)
