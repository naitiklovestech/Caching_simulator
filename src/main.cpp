#include "CacheSimulator.h"  // This will include HybridCache.h indirectly
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>  // for std::stoul, std::stod

void print_usage() {
    std::cout << "Usage: cache_simulator [options]\n"
              << "Options:\n"
              << "  -s, --size <size>       Cache size (default: 4)\n"
              << "  -a, --alpha <weight>    Frequency weight (default: 0.5)\n"
              << "  -b, --beta <weight>     Recency weight (default: 0.5)\n"
              << "  -n, --num-accesses <n>  Number of accesses (default: 20)\n"
              << "  -i, --interactive       Enable interactive test mode\n"
              << "  -h, --help              Show this help message\n";
}

int main(int argc, char* argv[]) {
    // Default parameters
    size_t cache_size = 4;
    double alpha = 0.5;
    double beta = 0.5;
    size_t num_accesses = 20;
    bool interactive = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        } else if (arg == "-s" || arg == "--size") {
            if (++i < argc) cache_size = std::stoul(argv[i]);
        } else if (arg == "-a" || arg == "--alpha") {
            if (++i < argc) alpha = std::stod(argv[i]);
        } else if (arg == "-b" || arg == "--beta") {
            if (++i < argc) beta = std::stod(argv[i]);
        } else if (arg == "-n" || arg == "--num-accesses") {
            if (++i < argc) num_accesses = std::stoul(argv[i]);
        } else if (arg == "-i" || arg == "--interactive") {
            interactive = true;
        }
    }

    // Create cache simulator
    CacheSimulator simulator(cache_size, alpha, beta);

    // Add some example processes with different access patterns
    simulator.add_process({1, 2, 3, 4, 5});     // Sequential access
    simulator.add_process({1, 1, 2, 2, 3});     // Repeated access
    simulator.add_process({5, 4, 3, 2, 1});     // Reverse sequential
    simulator.add_process({1, 3, 5, 2, 4});     // Interleaved access

    // Run simulation
    std::cout << "Starting cache simulation with:\n"
              << "  Cache size: " << cache_size << "\n"
              << "  Alpha (frequency weight): " << alpha << "\n"
              << "  Beta (recency weight): " << beta << "\n"
              << "  Number of accesses: " << num_accesses << "\n"
              << "  Interactive mode: " << (interactive ? "enabled" : "disabled") << "\n\n";

    simulator.run_simulation(num_accesses, interactive);

    return 0;
} 