#pragma once

#include <istream>
#include <ostream>
#include <string>

namespace core {

/**
 * @brief Strategy interface for compression algorithms.
 * 
 * Demonstrates the Strategy design pattern, allowing the system to switch
 * between different compression algorithms (like Huffman, LZW) interchangeably.
 */
class ICompressor {
public:
    virtual ~ICompressor() = default;

    /**
     * @brief Compresses data from the input stream and writes to the output stream.
     * 
     * @param input Stream containing raw original data.
     * @param output Stream where compressed data will be written.
     */
    virtual void compress(std::istream& input, std::ostream& output) = 0;

    /**
     * @brief Decompresses data from the input stream and writes to the output stream.
     * 
     * @param input Stream containing structured compressed data.
     * @param output Stream where reconstructed original data will be written.
     */
    virtual void decompress(std::istream& input, std::ostream& output) = 0;

    /**
     * @brief Get the name of the compression algorithm.
     * 
     * @return std::string Name of the algorithm (e.g., "Huffman", "LZW").
     */
    [[nodiscard]] virtual std::string getAlgorithmName() const = 0;
};

} // namespace core
