qt_add_executable(${PROJECT_NAME}
    main.cpp
    utils.h
    MainWindow.ui
    MainWindow.h
    MainWindow.cpp
    ImageLabel.h
    ImageLabel.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
    core
    asmimpl
    cimpl
    Qt::Core
    Qt::Gui
    Qt6::Widgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_precompile_headers(${PROJECT_NAME} PRIVATE)