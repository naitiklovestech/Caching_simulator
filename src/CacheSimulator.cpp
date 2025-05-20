#include "CacheSimulator.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <thread>

Process::Process(int id, const std::vector<int>& pages)
    : process_id(id), page_sequence(pages), current_index(0) {}

int Process::get_next_page() {
    if (current_index >= page_sequence.size()) {
        current_index = 0;  // Wrap around
    }
    return page_sequence[current_index++];
}

int Process::get_id() const {
    return process_id;
}

void Process::reset() {
    current_index = 0;
}

CacheSimulator::CacheSimulator(size_t cache_size, double alpha, double beta)
    : cache(std::make_unique<HybridCache<int, int>>(cache_size, alpha, beta)),
      total_accesses(0), hits(0), misses(0), evictions(0), test_mode(false) {
    start_time = std::chrono::steady_clock::now();
}

void CacheSimulator::add_process(const std::vector<int>& page_sequence) {
    processes.emplace_back(processes.size() + 1, page_sequence);
}

void CacheSimulator::print_header() const {
    std::cout << std::left << std::setw(10) << "Process" 
              << std::setw(10) << "Page" 
              << std::setw(10) << "Result" 
              << std::setw(10) << "Evicted" << "\n";
    std::cout << std::string(40, '-') << "\n";
}

void CacheSimulator::print_stats() const {
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::cout << "\nCache Statistics:\n";
    std::cout << std::string(40, '-') << "\n";
    std::cout << std::left << std::setw(20) << "Total Accesses" << ": " << total_accesses << "\n";
    std::cout << std::left << std::setw(20) << "Cache Hits" << ": " << hits << "\n";
    std::cout << std::left << std::setw(20) << "Cache Misses" << ": " << misses << "\n";
    std::cout << std::left << std::setw(20) << "Evictions" << ": " << evictions << "\n";
    std::cout << std::left << std::setw(20) << "Hit Ratio" << ": " 
              << std::fixed << std::setprecision(2) 
              << (static_cast<double>(hits) / total_accesses * 100) << "%\n";
    std::cout << std::left << std::setw(20) << "Simulation Time" << ": " 
              << duration.count() << "ms\n";
    std::cout << std::string(40, '-') << "\n";
}

void CacheSimulator::print_cache_state() const {
    std::cout << "\nCurrent Cache State:\n";
    std::cout << std::string(40, '-') << "\n";
    std::cout << std::left << std::setw(10) << "Page" 
              << std::setw(15) << "Frequency" 
              << std::setw(15) << "Last Access" << "\n";
    std::cout << std::string(40, '-') << "\n";
    
    // TODO: Implement cache state display
    // This would require adding methods to HybridCache to expose this information
    std::cout << "Cache state display not implemented yet\n";
}

bool CacheSimulator::handle_user_prediction(int page) {
    if (!test_mode) return true;
    
    std::cout << "Predict which page will be evicted (or -1 if none): ";
    int prediction;
    std::cin >> prediction;
    
    // Basic prediction logic
    if (prediction == -1) {
        std::cout << "Prediction: No page will be evicted\n";
        return true;
    }
    
    // Check if the predicted page is in the cache
    bool correct = !cache->contains(prediction);
    std::cout << "Prediction " << (correct ? "correct" : "incorrect") << "!\n";
    return true;
}

void CacheSimulator::run_simulation(size_t num_accesses, bool interactive) {
    test_mode = interactive;
    print_header();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> process_dist(0, processes.size() - 1);

    for (size_t i = 0; i < num_accesses; ++i) {
        // Select a random process
        int process_idx = process_dist(gen);
        Process& process = processes[process_idx];
        
        // Get next page to access
        int page = process.get_next_page();
        
        // Try to get the page from cache
        int value;
        bool hit = cache->get(page, value);
        
        if (hit) {
            hits++;
        } else {
            misses++;
            // Cache miss - add to cache
            cache->put(page, page);
        }
        
        total_accesses++;

        // Print access information
        std::cout << std::left << std::setw(10) << process.get_id()
                  << std::setw(10) << page
                  << std::setw(10) << (hit ? "HIT" : "MISS")
                  << std::setw(10) << (hit ? "-" : "N/A") << "\n";

        if (interactive) {
            print_cache_state();
            if (!handle_user_prediction(page)) {
                break;
            }
        }

        // Add a small delay to make the simulation more visible
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    print_stats();
}

void CacheSimulator::set_test_mode(bool enabled) {
    test_mode = enabled;
}

double CacheSimulator::get_hit_ratio() const {
    return static_cast<double>(hits) / total_accesses;
}

size_t CacheSimulator::get_total_accesses() const {
    return total_accesses;
}

size_t CacheSimulator::get_hits() const {
    return hits;
}

size_t CacheSimulator::get_misses() const {
    return misses;
}

size_t CacheSimulator::get_evictions() const {
    return evictions;
}

double CacheSimulator::get_simulation_time() const {
    auto end_time = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
} 