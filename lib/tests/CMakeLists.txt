find_package(GTest REQUIRED)

enable_testing()

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

add_executable(tests ${TEST_SOURCES})
target_include_directories(tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)
target_link_libraries(tests PUBLIC
    GTest::gtest
    GTest::gtest_main
    ${INSTALL_TARGET_LIB_BINARY}
)

include(GoogleTest)
gtest_discover_tests(tests)