qt_add_executable(${PROJECT_NAME}
    main.cpp
    MainWindow.ui
    MainWindow.h
    MainWindow.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    cimpl
    Qt::Core
    Qt::Gui
    Qt6::Widgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release
)

target_precompile_headers(${PROJECT_NAME} PRIVATE)