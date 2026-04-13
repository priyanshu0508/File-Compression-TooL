#pragma once

#include "core/ICompressor.hpp"

namespace algorithms {

/**
 * @brief Lempel-Ziv-Welch (LZW) compression algorithm implementation.
 */
class LZW : public core::ICompressor {
public:
    void compress(std::istream& input, std::ostream& output) override;
    void decompress(std::istream& input, std::ostream& output) override;

    [[nodiscard]] std::string getAlgorithmName() const override {
        return "LZW";
    }
};

} // namespace algorithms
