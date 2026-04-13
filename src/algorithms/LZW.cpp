#include "LZW.hpp"
#include "core/BitStream.hpp"
#include <unordered_map>
#include <vector>
#include <string>

namespace algorithms {

void LZW::compress(std::istream& input, std::ostream& output) {
    std::unordered_map<std::string, uint32_t> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[std::string(1, static_cast<char>(i))] = i;
    }

    std::string current;
    char next;
    uint32_t nextCode = 256;
    
    // We use 12-bit codes for simplicity in this implementation, 
    // but a production LZW would use variable bit widths or bit-packing.
    // For this demonstration, we'll write 16-bit codes to keep it simple but functional.
    
    while (input.get(next)) {
        std::string extended = current + next;
        if (dictionary.count(extended)) {
            current = extended;
        } else {
            uint32_t code = dictionary[current];
            output.write(reinterpret_cast<const char*>(&code), sizeof(uint16_t));
            
            if (nextCode < 65535) {
                dictionary[extended] = nextCode++;
            }
            current = std::string(1, next);
        }
    }

    if (!current.empty()) {
        uint32_t code = dictionary[current];
        output.write(reinterpret_cast<const char*>(&code), sizeof(uint16_t));
    }
}

void LZW::decompress(std::istream& input, std::ostream& output) {
    std::unordered_map<uint32_t, std::string> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = std::string(1, static_cast<char>(i));
    }

    uint16_t code;
    if (!input.read(reinterpret_cast<char*>(&code), sizeof(uint16_t))) return;

    std::string current = dictionary[code];
    output << current;

    uint32_t nextCode = 256;
    uint16_t nextIn;
    
    while (input.read(reinterpret_cast<char*>(&nextIn), sizeof(uint16_t))) {
        std::string entry;
        if (dictionary.count(nextIn)) {
            entry = dictionary[nextIn];
        } else if (nextIn == nextCode) {
            entry = current + current[0];
        } else {
            throw std::runtime_error("LZW decompress: Invalid code");
        }

        output << entry;
        
        if (nextCode < 65535) {
            dictionary[nextCode++] = current + entry[0];
        }
        current = entry;
    }
}

} // namespace algorithms
