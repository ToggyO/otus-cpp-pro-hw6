file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true src/*.h)

add_subdirectory(tests)

add_library(${LIB_BINARY} STATIC ${SOURCES})

set_target_properties(${LIB_BINARY} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${LIB_BINARY} INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)