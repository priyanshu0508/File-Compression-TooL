#pragma once

#include "core/ICompressor.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <cstdint>

namespace algorithms {

/**
 * @brief Huffman coding algorithm implementation.
 */
class Huffman : public core::ICompressor {
public:
    void compress(std::istream& input, std::ostream& output) override;
    void decompress(std::istream& input, std::ostream& output) override;

    [[nodiscard]] std::string getAlgorithmName() const override {
        return "Huffman";
    }

private:
    struct Node {
        uint8_t character;
        uint64_t frequency;
        uint8_t minChar; // Used for stable tie-breaking during tree reconstruction
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(uint8_t charac, uint64_t freq);
        Node(std::shared_ptr<Node> l, std::shared_ptr<Node> r);
        
        bool isLeaf() const;
    };

    struct CompareNode {
        bool operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const {
            if (lhs->frequency != rhs->frequency) {
                return lhs->frequency > rhs->frequency; // Primary: Frequency (Min-heap)
            }
            return lhs->minChar > rhs->minChar; // Stable Tie-breaker: Minimum character in subtree
        }
    };

    void buildCodes(const std::shared_ptr<Node>& node, const std::string& currentCode, std::unordered_map<uint8_t, std::string>& huffmanCodes);
    void writeHeader(std::ostream& output, const std::unordered_map<uint8_t, uint64_t>& frequencies);
    std::unordered_map<uint8_t, uint64_t> readHeader(std::istream& input);
};

} // namespace algorithms
