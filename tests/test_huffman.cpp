#include <gtest/gtest.h>
#include <sstream>
#include "algorithms/Huffman.hpp"

using namespace algorithms;

TEST(HuffmanTest, SimpleTextCompression) {
    std::string testData = "this is a test for huffman coding";
    std::stringstream input(testData);
    std::stringstream compressed(std::ios::binary | std::ios::in | std::ios::out);
    std::stringstream decompressed(std::ios::binary | std::ios::in | std::ios::out);

    Huffman compressor;
    
    // Compress
    compressor.compress(input, compressed);
    
    // Decompress
    compressed.seekg(0);
    compressor.decompress(compressed, decompressed);

    EXPECT_EQ(decompressed.str(), testData);
    EXPECT_LT(compressed.str().size(), testData.size() + 256); // Header might be larger for very small strings
}

TEST(HuffmanTest, EmptyFile) {
    std::string testData = "";
    std::stringstream input(testData);
    std::stringstream compressed(std::ios::binary | std::ios::in | std::ios::out);
    std::stringstream decompressed(std::ios::binary | std::ios::in | std::ios::out);

    Huffman compressor;
    compressor.compress(input, compressed);
    
    compressed.seekg(0);
    compressor.decompress(compressed, decompressed);

    EXPECT_EQ(decompressed.str(), "");
}

TEST(HuffmanTest, SingleCharacterFile) {
    std::string testData = "aaaaa";
    std::stringstream input(testData);
    std::stringstream compressed(std::ios::binary | std::ios::in | std::ios::out);
    std::stringstream decompressed(std::ios::binary | std::ios::in | std::ios::out);

    Huffman compressor;
    compressor.compress(input, compressed);
    
    compressed.seekg(0);
    compressor.decompress(compressed, decompressed);

    EXPECT_EQ(decompressed.str(), testData);
}

TEST(HuffmanTest, BinaryData) {
    std::string testData;
    for(int i=0; i<256; ++i) {
        testData += static_cast<char>(i);
    }
    
    std::stringstream input(testData);
    std::stringstream compressed(std::ios::binary | std::ios::in | std::ios::out);
    std::stringstream decompressed(std::ios::binary | std::ios::in | std::ios::out);

    Huffman compressor;
    compressor.compress(input, compressed);
    
    compressed.seekg(0);
    compressor.decompress(compressed, decompressed);

    EXPECT_EQ(decompressed.str(), testData);
}
