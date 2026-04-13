#include "Huffman.hpp"
#include "core/BitStream.hpp"
#include <queue>
#include <stdexcept>
#include <iostream>

namespace algorithms {

Huffman::Node::Node(uint8_t charac, uint64_t freq)
    : character(charac), frequency(freq), minChar(charac), left(nullptr), right(nullptr) {}

Huffman::Node::Node(std::shared_ptr<Node> l, std::shared_ptr<Node> r)
    : character(0), frequency(l->frequency + r->frequency), 
      minChar(std::min(l->minChar, r->minChar)), left(std::move(l)), right(std::move(r)) {}

bool Huffman::Node::isLeaf() const {
    return left == nullptr && right == nullptr;
}

void Huffman::buildCodes(const std::shared_ptr<Node>& node, const std::string& currentCode, std::unordered_map<uint8_t, std::string>& huffmanCodes) {
    if (!node) return;

    if (node->isLeaf()) {
        huffmanCodes[node->character] = currentCode.empty() ? "0" : currentCode;
    } else {
        buildCodes(node->left, currentCode + "0", huffmanCodes);
        buildCodes(node->right, currentCode + "1", huffmanCodes);
    }
}

void Huffman::writeHeader(std::ostream& output, const std::unordered_map<uint8_t, uint64_t>& frequencies) {
    // Determine number of unique characters
    uint16_t size = static_cast<uint16_t>(frequencies.size());
    output.write(reinterpret_cast<const char*>(&size), sizeof(size));

    // Write frequencies (char, freq pairs)
    for (const auto& [ch, freq] : frequencies) {
        output.write(reinterpret_cast<const char*>(&ch), sizeof(ch));
        output.write(reinterpret_cast<const char*>(&freq), sizeof(freq));
    }
}

std::unordered_map<uint8_t, uint64_t> Huffman::readHeader(std::istream& input) {
    std::unordered_map<uint8_t, uint64_t> frequencies;
    uint16_t size = 0;
    
    if (!input.read(reinterpret_cast<char*>(&size), sizeof(size))) {
        return frequencies; // Empty file or error
    }

    for (uint16_t i = 0; i < size; ++i) {
        uint8_t ch;
        uint64_t freq;
        input.read(reinterpret_cast<char*>(&ch), sizeof(ch));
        input.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        frequencies[ch] = freq;
    }

    return frequencies;
}

void Huffman::compress(std::istream& input, std::ostream& output) {
    std::unordered_map<uint8_t, uint64_t> frequencies;
    
    // Pass 1: compute frequencies
    char c;
    uint64_t totalChars = 0;
    while (input.get(c)) {
        frequencies[static_cast<uint8_t>(c)]++;
        totalChars++;
    }

    if (frequencies.empty()) return;

    // Write header
    writeHeader(output, frequencies);
    
    // Also write the total character count so decompressor knows exactly when to stop!
    output.write(reinterpret_cast<const char*>(&totalChars), sizeof(totalChars));

    // Build priority queue
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNode> pq;
    for (const auto& [ch, freq] : frequencies) {
        pq.push(std::make_shared<Node>(ch, freq));
    }

    // Build Huffman tree
    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();
        pq.push(std::make_shared<Node>(left, right));
    }

    auto root = pq.top();

    // Generate codes
    std::unordered_map<uint8_t, std::string> huffmanCodes;
    buildCodes(root, "", huffmanCodes);

    // Pass 2: encode data
    input.clear();
    input.seekg(0, std::ios::beg);

    core::BitOutputStream bitOut(output);
    while (input.get(c)) {
        const std::string& code = huffmanCodes[static_cast<uint8_t>(c)];
        for (char bitChar : code) {
            bitOut.writeBit(bitChar == '1' ? 1 : 0);
        }
    }
}

void Huffman::decompress(std::istream& input, std::ostream& output) {
    auto frequencies = readHeader(input);
    if (frequencies.empty()) return;

    uint64_t totalChars = 0;
    input.read(reinterpret_cast<char*>(&totalChars), sizeof(totalChars));

    // Rebuild tree
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNode> pq;
    for (const auto& [ch, freq] : frequencies) {
        pq.push(std::make_shared<Node>(ch, freq));
    }

    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();
        pq.push(std::make_shared<Node>(left, right));
    }

    auto root = pq.top();

    // Decode utilizing bitstream
    core::BitInputStream bitIn(input);
    auto currentNode = root;
    uint64_t decodedChars = 0;

    while (decodedChars < totalChars) {
        if (currentNode->isLeaf()) {
            output.put(static_cast<char>(currentNode->character));
            decodedChars++;
            currentNode = root;
            continue; // Node could be root itself (single character file)
        }

        int bit = bitIn.readBit();
        if (bit == -1) break;

        if (bit == 0) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        if (currentNode->isLeaf()) {
            output.put(static_cast<char>(currentNode->character));
            decodedChars++;
            currentNode = root;
        }
    }
}

} // namespace algorithms
