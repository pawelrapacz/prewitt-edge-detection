find_package(Python3 REQUIRED COMPONENTS Interpreter)

# reference prewitt results generation

set(INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/input)
set(EXPECT_DIR ${CMAKE_CURRENT_BINARY_DIR}/expect)
set(REFERENCE_STAMP ${CMAKE_CURRENT_BINARY_DIR}/reference.stamp)
set(REFERENCE_AMOUNT 1.0)

file(GLOB INPUT_FILES LIST_DIRECTORIES false CONFIGURE_DEPENDS input/*)

# this command generates the reference results for testing
add_custom_command(
    OUTPUT ${REFERENCE_STAMP}
    COMMAND
        ${Python3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/reference.py
        --input-dir ${INPUT_DIR}
        --expect-dir ${EXPECT_DIR}
        --amount ${REFERENCE_AMOUNT}
    COMMAND
        ${CMAKE_COMMAND} -E touch ${REFERENCE_STAMP}
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        reference.py
        ${INPUT_FILES}
    COMMENT "Generating reference prewitt results"
    USES_TERMINAL
    VERBATIM
)

# target that tests can depend on
# it will be rebuild whenever the above command dependencies change
add_custom_target(generate_reference
    DEPENDS
        ${REFERENCE_STAMP}
)

unset(REFERENCE_STAMP)
unset(INPUT_FILES)


# TESTS

add_executable(tests
    tests_main.cpp
    prewitt_impl.h
    core_tests.cpp
    reference_tests.cpp
)

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_dependencies(tests generate_reference)

target_link_libraries(tests PRIVATE
    doctest::doctest
    core
    cimpl
    asmimpl
    Qt::Gui
)

target_compile_definitions(tests PRIVATE
    TESTS_REFERENCE_INPUT_DIR="${INPUT_DIR}"
    TESTS_REFERENCE_EXPECT_DIR="${EXPECT_DIR}"
    TESTS_AMOUNT=${REFERENCE_AMOUNT}
)

doctest_discover_tests(tests)

unset(INPUT_DIR)
unset(EXPECT_DIR)
unset(REFERENCE_AMOUNT)
