#pragma once

#include "HybridCache.h"
#include <vector>
#include <string>
#include <memory>
#include <chrono>

class Process {
private:
    int process_id;
    std::vector<int> page_sequence;
    size_t current_index;

public:
    Process(int id, const std::vector<int>& pages);
    int get_next_page();
    int get_id() const;
    void reset();
};

class CacheSimulator {
private:
    std::unique_ptr<HybridCache<int, int>> cache;
    std::vector<Process> processes;
    size_t total_accesses;
    size_t hits;
    size_t misses;
    size_t evictions;
    bool test_mode;
    std::chrono::steady_clock::time_point start_time;

    void print_header() const;
    void print_stats() const;
    void print_cache_state() const;
    bool handle_user_prediction(int page);

public:
    CacheSimulator(size_t cache_size, double alpha = 0.5, double beta = 0.5);
    
    void add_process(const std::vector<int>& page_sequence);
    void run_simulation(size_t num_accesses, bool interactive = false);
    void set_test_mode(bool enabled);
    
    // Statistics
    double get_hit_ratio() const;
    size_t get_total_accesses() const;
    size_t get_hits() const;
    size_t get_misses() const;
    size_t get_evictions() const;
    double get_simulation_time() const;
}; 