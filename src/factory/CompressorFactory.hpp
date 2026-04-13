#pragma once

#include "core/ICompressor.hpp"
#include <memory>
#include <string>
#include <vector>

namespace factory {

/**
 * @brief Factory class to create compressor instances.
 * 
 * Demonstrates the Factory design pattern, decoupling the main logic
 * from the specific algorithm implementations.
 */
class CompressorFactory {
public:
    /**
     * @brief Create a compressor based on the algorithm name.
     * 
     * @param algorithm Name of the algorithm ("huffman", "lzw").
     * @return std::unique_ptr<core::ICompressor> Instance of the compressor.
     * @throws std::invalid_argument if the algorithm name is unknown.
     */
    static std::unique_ptr<core::ICompressor> createCompressor(const std::string& algorithm);

    /**
     * @brief Get a list of supported algorithm names.
     */
    static std::vector<std::string> getSupportedAlgorithms();
};

} // namespace factory
