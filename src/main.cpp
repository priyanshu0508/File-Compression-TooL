#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

#include <cxxopts.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <indicators/cursor_control.hpp>
#include <indicators/progress_bar.hpp>
#include <indicators/indeterminate_progress_bar.hpp>
#include <thread>
#include <cstdint>

#include "factory/CompressorFactory.hpp"
#include "utils/Logger.hpp"

namespace fs = std::filesystem;

void runCompression(const std::string& inputPath, const std::string& outputPath, const std::string& algo) {
    auto compressor = factory::CompressorFactory::createCompressor(algo);
    
    std::ifstream input(inputPath, std::ios::binary);
    if (!input) throw std::runtime_error("Cannot open input file: " + inputPath);
    
    std::ofstream output(outputPath, std::ios::binary);
    if (!output) throw std::runtime_error("Cannot open output file: " + outputPath);

    uint64_t inputSize = fs::file_size(inputPath);
    utils::Logger::info(fmt::format("Original Size: {} bytes", inputSize));
    utils::Logger::info("Compressing...");

    // Show a sleek indeterminate progress bar during processing
    using namespace indicators;
    IndeterminateProgressBar bar{
        option::BarWidth{40},
        option::Start{"["},
        option::Fill{"·"},
        option::Lead{"<==>"},
        option::End{"]"},
        option::PostfixText{fmt::format("Running {} Algorithm", compressor->getAlgorithmName())},
        option::ForegroundColor{Color::yellow}
    };

    std::thread barThread([&bar]() {
        while (!bar.is_completed()) {
            bar.tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    auto start = std::chrono::high_resolution_clock::now();
    compressor->compress(input, output);
    auto end = std::chrono::high_resolution_clock::now();
    
    bar.mark_as_completed();
    barThread.join();

    output.close();
    uint64_t outputSize = fs::file_size(outputPath);
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    utils::Logger::success("Compression finished!");
    utils::Logger::info(fmt::format("Compressed Size: {} bytes", outputSize));
    
    double ratio = (1.0 - (double)outputSize / inputSize) * 100.0;
    utils::Logger::info(fmt::format("Space Saved: {:.2f}%", ratio));
    utils::Logger::info(fmt::format("Time Taken: {}ms", duration));
}

void runDecompression(const std::string& inputPath, const std::string& outputPath, const std::string& algo) {
    auto compressor = factory::CompressorFactory::createCompressor(algo);
    
    std::ifstream input(inputPath, std::ios::binary);
    if (!input) throw std::runtime_error("Cannot open input file: " + inputPath);
    
    std::ofstream output(outputPath, std::ios::binary);
    if (!output) throw std::runtime_error("Cannot open output file: " + outputPath);

    utils::Logger::info("Decompressing...");

    indicators::show_console_cursor(false);
    indicators::IndeterminateProgressBar bar{
        indicators::option::BarWidth{40},
        indicators::option::Start{"["},
        indicators::option::Fill{"·"},
        indicators::option::Lead{"<==>"},
        indicators::option::End{"]"},
        indicators::option::PostfixText{"Reconstructing Original Data"},
        indicators::option::ForegroundColor{indicators::Color::cyan}
    };

    std::thread barThread([&bar]() {
        while (!bar.is_completed()) {
            bar.tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    auto start = std::chrono::high_resolution_clock::now();
    compressor->decompress(input, output);
    auto end = std::chrono::high_resolution_clock::now();

    bar.mark_as_completed();
    barThread.join();
    indicators::show_console_cursor(true);

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    utils::Logger::success("Decompression finished!");
    utils::Logger::info(fmt::format("Time Taken: {}ms", duration));
}

int main(int argc, char* argv[]) {
    cxxopts::Options options("compressor", "Advanced C++ File Compression Tool");

    options.add_options()
        ("c,compress", "Compress file", cxxopts::value<bool>())
        ("d,decompress", "Decompress file", cxxopts::value<bool>())
        ("i,input", "Input file", cxxopts::value<std::string>())
        ("o,output", "Output file", cxxopts::value<std::string>())
        ("a,algo", "Algorithm (huffman/lzw)", cxxopts::value<std::string>()->default_value("huffman"))
        ("h,help", "Print usage")
    ;

    try {
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        bool compress = result.count("compress");
        bool decompress = result.count("decompress");

        if (compress == decompress) {
            utils::Logger::error("Please specify either --compress (-c) or --decompress (-d)");
            return 1;
        }

        if (!result.count("input") || !result.count("output")) {
            utils::Logger::error("Input and output files are required.");
            return 1;
        }

        std::string inputPath = result["input"].as<std::string>();
        std::string outputPath = result["output"].as<std::string>();
        std::string algo = result["algo"].as<std::string>();

        utils::Logger::header("FILE COMPRESSION TOOL");

        if (compress) {
            runCompression(inputPath, outputPath, algo);
        } else {
            runDecompression(inputPath, outputPath, algo);
        }

    } catch (const std::exception& e) {
        utils::Logger::error(e.what());
        return 1;
    }

    return 0;
}
