include(GoogleTest)

# We use gtest_main so we don't have to write our own main function for tests
add_executable(test_suite
    test_bitstream.cpp
    test_huffman.cpp
    test_lzw.cpp
)

target_link_libraries(test_suite
    PRIVATE
        core
        algorithms
        GTest::gtest_main
)

target_compile_options(test_suite PRIVATE -Wall -Wextra -Wpedantic)

gtest_discover_tests(test_suite)
