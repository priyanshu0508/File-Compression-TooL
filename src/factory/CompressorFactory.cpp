#include "CompressorFactory.hpp"
#include "algorithms/Huffman.hpp"
#include "algorithms/LZW.hpp"
#include <stdexcept>
#include <algorithm>

namespace factory {

std::unique_ptr<core::ICompressor> CompressorFactory::createCompressor(const std::string& algorithm) {
    std::string lowerAlgo = algorithm;
    std::transform(lowerAlgo.begin(), lowerAlgo.end(), lowerAlgo.begin(), ::tolower);

    if (lowerAlgo == "huffman") {
        return std::make_unique<algorithms::Huffman>();
    } else if (lowerAlgo == "lzw") {
        return std::make_unique<algorithms::LZW>();
    } else {
        throw std::invalid_argument("Unknown compression algorithm: " + algorithm);
    }
}

std::vector<std::string> CompressorFactory::getSupportedAlgorithms() {
    return {"Huffman", "LZW"};
}

} // namespace factory
