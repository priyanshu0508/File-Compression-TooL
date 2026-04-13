# Core library for bitwise I/O
add_library(core
    core/BitStream.cpp
)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(core PRIVATE -Wall -Wextra -Wpedantic)

# Algorithms library
add_library(algorithms
    algorithms/Huffman.cpp
    algorithms/LZW.cpp
    factory/CompressorFactory.cpp
)
target_include_directories(algorithms PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(algorithms PUBLIC core)
target_compile_options(algorithms PRIVATE -Wall -Wextra -Wpedantic)

# Build the main executable
add_executable(compressor
    main.cpp
)

target_link_libraries(compressor
    PRIVATE
        core
        algorithms
        cxxopts::cxxopts
        fmt::fmt
        indicators::indicators
)
target_compile_options(compressor PRIVATE -Wall -Wextra -Wpedantic)

# For testing visibility
# (In a real project you'd export these cleanly or build test objects, here linking to a static/object library is fine)
