#include "BitStream.hpp"

namespace core {

// --- BitOutputStream ---

BitOutputStream::BitOutputStream(std::ostream& outStream)
    : m_out(outStream), m_buffer(0), m_bitCount(0) {
}

BitOutputStream::~BitOutputStream() {
    flush();
}

void BitOutputStream::writeBit(int bit) {
    if (bit) {
        // Set the bit at the correct position (from MSB to LSB convention)
        // Shift left by (7 - m_bitCount)
        m_buffer |= (1 << (7 - m_bitCount));
    }
    m_bitCount++;
    
    if (m_bitCount == 8) {
        m_out.put(static_cast<char>(m_buffer));
        m_buffer = 0;
        m_bitCount = 0;
    }
}

void BitOutputStream::writeBits(uint32_t value, size_t numBits) {
    for (size_t i = 0; i < numBits; ++i) {
        // Read bits from MSB to LSB within the requested range
        int bit = (value >> (numBits - 1 - i)) & 1;
        writeBit(bit);
    }
}

void BitOutputStream::writeByte(uint8_t byte) {
    writeBits(byte, 8);
}

void BitOutputStream::flush() {
    if (m_bitCount > 0) {
        m_out.put(static_cast<char>(m_buffer));
        m_buffer = 0;
        m_bitCount = 0;
    }
}

// --- BitInputStream ---

BitInputStream::BitInputStream(std::istream& inStream)
    : m_in(inStream), m_buffer(0), m_bitCount(0) {
}

int BitInputStream::readBit() {
    if (m_bitCount == 0) {
        int nextByte = m_in.get();
        if (nextByte == std::char_traits<char>::eof()) {
            return -1; // EOF
        }
        m_buffer = static_cast<uint8_t>(nextByte);
        m_bitCount = 8;
    }

    int bit = (m_buffer >> (m_bitCount - 1)) & 1;
    m_bitCount--;
    return bit;
}

int BitInputStream::readByte() {
    int value = 0;
    for (int i = 0; i < 8; ++i) {
        int bit = readBit();
        if (bit == -1) {
            // If we are halfway through a byte and hit EOF, we just return what we have
            // But if we haven't read anything, return EOF.
            if (i == 0) return -1;
            break;
        }
        value = (value << 1) | bit;
    }
    return value;
}

} // namespace core
