#pragma once

#include <iostream>
#include <cstdint>
#include <cstddef>

namespace core {

/**
 * @brief Output stream for writing individual bits.
 * 
 * Standard output streams operate on bytes. Compressing data effectively
 * requires writing 1s and 0s (bits). This class buffers bits into an 8-bit
 * byte, and flushes it to the underlying std::ostream when full.
 */
class BitOutputStream {
public:
    explicit BitOutputStream(std::ostream& outStream);
    ~BitOutputStream();

    // Prevent copying
    BitOutputStream(const BitOutputStream&) = delete;
    BitOutputStream& operator=(const BitOutputStream&) = delete;

    /**
     * @brief Write a single bit (0 or 1) to the stream.
     */
    void writeBit(int bit);

    /**
     * @brief Write the lower `numBits` bits of `value` to the stream.
     */
    void writeBits(uint32_t value, size_t numBits);

    /**
     * @brief Write a full byte directly (bypassing bit buffer if possible to optimize,
     * or routing through writeBit).
     */
    void writeByte(uint8_t byte);

    /**
     * @brief Flush any remaining bits in the buffer to the stream.
     * 
     * Pads with 0s if the buffer is partially full.
     */
    void flush();

private:
    std::ostream& m_out;
    uint8_t m_buffer{0};
    uint8_t m_bitCount{0};
};


/**
 * @brief Input stream for reading individual bits.
 */
class BitInputStream {
public:
    explicit BitInputStream(std::istream& inStream);

    // Prevent copying
    BitInputStream(const BitInputStream&) = delete;
    BitInputStream& operator=(const BitInputStream&) = delete;

    /**
     * @brief Reads a single bit from the stream.
     * 
     * @return 0 or 1, or -1 if EOF is reached and the buffer is empty.
     */
    int readBit();

    /**
     * @brief Reads a full byte.
     * 
     * @return The 8-bit value, or -1 if EOF is reached.
     */
    int readByte();

private:
    std::istream& m_in;
    uint8_t m_buffer{0};
    uint8_t m_bitCount{0};
};

} // namespace core
