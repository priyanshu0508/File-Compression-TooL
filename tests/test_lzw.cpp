#include <gtest/gtest.h>
#include <sstream>
#include "algorithms/LZW.hpp"

using namespace algorithms;

TEST(LZWTest, SimpleTextCompression) {
    std::string testData = "TOBEORNOTTOBEORTOBEORNOT";
    std::stringstream input(testData);
    std::stringstream compressed(std::ios::binary | std::ios::in | std::ios::out);
    std::stringstream decompressed(std::ios::binary | std::ios::in | std::ios::out);

    LZW compressor;
    
    // Compress
    compressor.compress(input, compressed);
    
    // Decompress
    compressed.seekg(0);
    compressor.decompress(compressed, decompressed);

    EXPECT_EQ(decompressed.str(), testData);
}

TEST(LZWTest, LongRepeatingText) {
    std::string testData = "";
    for(int i=0; i<100; ++i) testData += "ABCABCABC";
    
    std::stringstream input(testData);
    std::stringstream compressed(std::ios::binary | std::ios::in | std::ios::out);
    std::stringstream decompressed(std::ios::binary | std::ios::in | std::ios::out);

    LZW compressor;
    compressor.compress(input, compressed);
    
    compressed.seekg(0);
    compressor.decompress(compressed, decompressed);

    EXPECT_EQ(decompressed.str(), testData);
    // LZW should be very efficient for repeating patterns
    EXPECT_LT(compressed.str().size(), testData.size());
}

TEST(LZWTest, CaseWithNewCodes) {
    // Specifically testing the logic where a code is not yet in dictionary during decompression
    // This happens in the "cScSc" pattern
    std::string testData = "banana_bandana"; 
    std::stringstream input(testData);
    std::stringstream compressed(std::ios::binary | std::ios::in | std::ios::out);
    std::stringstream decompressed(std::ios::binary | std::ios::in | std::ios::out);

    LZW compressor;
    compressor.compress(input, compressed);
    
    compressed.seekg(0);
    compressor.decompress(compressed, decompressed);

    EXPECT_EQ(decompressed.str(), testData);
}
