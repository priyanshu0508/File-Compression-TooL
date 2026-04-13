# File Compression Tool

A production-grade file compression utility built with **Modern C++ (C++17)**. This tool implements multiple compression strategies and features a polished, recruiter-ready command-line interface.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Build](https://img.shields.io/badge/build-CMake-orange.svg)

## ✨ Key Features

-   **Dual Compression Algorithms:** Switch between **Huffman Coding** (entropy-based) and **LZW** (dictionary-based) strategies.
-   **Systems-Level Bit Manipulation:** Bespoke `BitInputStream` and `BitOutputStream` classes for efficient bit-level I/O, bypassing the byte-aligned limitations of standard streams.
-   **Software Design Patterns:**
    -   **Strategy Pattern:** Encapsulates compression algorithms for interchangeable use.
    -   **Factory Pattern:** Decouples algorithm selection from main application logic.
-   **Premium CLI Experience:** 
    -   Animated, indeterminate progress bars (via `indicators`).
    -   Colorized, high-standard logging (via `{fmt}`).
    -   Robust argument parsing (via `cxxopts`).
-   **Unit Tested:** Comprehensive test suite built with **GoogleTest** ensuring data integrity.

## 🛠 Tech Stack

-   **Language:** C++17
-   **Build System:** CMake
-   **Testing:** GoogleTest
-   **Libraries:** 
    -   `fmt`: Type-safe formatting and colors.
    -   `cxxopts`: Header-only CLI parsing.
    -   `indicators`: Activity spinners and progress bars.

## 🚀 Getting Started (MSYS2 UCRT64)

This project is optimized for the **MSYS2 UCRT64** environment.

### Prerequisites

From your MSYS2 UCRT64 terminal, ensure you have the necessary tools:
```bash
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja diffutils
```

### Building the Project

The project uses CMake's `FetchContent` to automatically download all dependencies (fmt, indicators, gtest, cxxopts).

```bash
mkdir build && cd build
cmake .. -G "Ninja"
cmake --build .
```

### Running Tests

Verify the core logic and bitstream handling:
```bash
# From the build directory
./tests/test_suite
```

## 📖 Usage

All commands should be run from the `build` directory.

### Huffman Compression
```bash
# Compress
./src/compressor -c -i ../README.md -o readme_huff.bin --algo huffman

# Decompress
./src/compressor -d -i readme_huff.bin -o restored_huff.md --algo huffman
```

### LZW Compression
```bash
# Compress
./src/compressor -c -i ../README.md -o readme_lzw.bin --algo lzw

# Decompress
./src/compressor -d -i readme_lzw.bin -o restored_lzw.md --algo lzw
```

### Verification
You can verify the files are identical using the `diff` command:
```bash
diff ../README.md restored_huff.md
```

### Arguments
| Short | Long | Description |
| :--- | :--- | :--- |
| `-c` | `--compress` | Set mode to compress |
| `-d` | `--decompress` | Set mode to decompress |
| `-i` | `--input` | Path to the input file |
| `-o` | `--output` | Path to the output file |
| `-a` | `--algo` | Algorithm: `huffman` (default) or `lzw` |
| `-h` | `--help` | Show usage help |

## 🏗 Architecture Details

### The BitStream Layer
Standard C++ `std::ostream` writes a minimum of 8 bits at a time. This project implements a custom buffering layer in `BitStream.cpp` that allows writing single bits. When the buffer hits 8 bits, it flushes a single byte to the disk. This is essential for Huffman coding where symbols vary in bit-length.

### The Strategy Pattern
By using an `ICompressor` interface, the system is completely decoupled. The `CompressorFactory` instantiates the requested algorithm at runtime, allowing for easy expansion to new algorithms like Deflate or BWT later.

---
*Developed with ❤️ to showcase professional C++ engineering.*
