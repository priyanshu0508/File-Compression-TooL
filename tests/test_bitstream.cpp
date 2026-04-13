#include <gtest/gtest.h>
#include <sstream>
#include "core/BitStream.hpp"

using namespace core;

TEST(BitStreamTest, WriteAndReadSingleBits) {
    std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
    
    {
        BitOutputStream out(ss);
        out.writeBit(1);
        out.writeBit(0);
        out.writeBit(1);
        out.writeBit(1);
        out.writeBit(0);
        out.writeBit(0);
        out.writeBit(1);
        out.writeBit(1); // 10110011 -> 0xB3 
    }

    {
        BitInputStream in(ss);
        EXPECT_EQ(in.readBit(), 1);
        EXPECT_EQ(in.readBit(), 0);
        EXPECT_EQ(in.readBit(), 1);
        EXPECT_EQ(in.readBit(), 1);
        EXPECT_EQ(in.readBit(), 0);
        EXPECT_EQ(in.readBit(), 0);
        EXPECT_EQ(in.readBit(), 1);
        EXPECT_EQ(in.readBit(), 1);
        EXPECT_EQ(in.readBit(), -1); // EOF
    }
}

TEST(BitStreamTest, WriteAndReadMultipleBits) {
    std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
    
    {
        BitOutputStream out(ss);
        // Write 13 (binary 1101) using 4 bits
        out.writeBits(13, 4);
        // Write 2 (binary 0010) using 4 bits
        out.writeBits(2, 4);
    } // Buffer flushes here (4+4 = 8 bits)

    {
        BitInputStream in(ss);
        EXPECT_EQ(in.readByte(), (13 << 4) | 2); // 11010010
        EXPECT_EQ(in.readBit(), -1);
    }
}

TEST(BitStreamTest, PartialByteFlush) {
    std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
    
    {
        BitOutputStream out(ss);
        out.writeBits(7, 3); // 111
    } // Expect flush to pad with 0s: 11100000 -> 0xE0

    {
        BitInputStream in(ss);
        EXPECT_EQ(in.readByte(), 0xE0);
    }
}
