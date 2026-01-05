cmake_minimum_required(VERSION 3.15)
project(transplant CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-unused-variable -Wno-unused-function")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG -DCOLOR -DERROR -DSUCCESS -DWARN -DINFO")

include_directories(${CMAKE_SOURCE_DIR}/include)

set(TRANSPLANT_SOURCES
    src/path_manager.cpp
    src/binary_io.cpp
    src/serializer.cpp
    src/deserializer.cpp
    src/command_line_parser.cpp
)

add_executable(transplant
    src/main.cpp
    ${TRANSPLANT_SOURCES}
)

find_library(CRITERION_LIB criterion)
if(CRITERION_LIB)
    add_executable(transplant_tests
        ${TRANSPLANT_SOURCES}
        src/test_adapter.cpp
        tests/test_common.cpp
        tests/transplant_tests.cpp
    )

    target_compile_options(transplant_tests PRIVATE -Wno-write-strings -Wno-unused-but-set-variable)
    target_link_libraries(transplant_tests criterion)

    enable_testing()
    add_test(NAME transplant_tests COMMAND transplant_tests)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
