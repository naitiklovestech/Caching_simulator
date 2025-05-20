#pragma once

#include <chrono>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>

template<typename Key, typename Value>
class HybridCache {
private:
    struct CacheEntry {
        Value value;
        size_t frequency;
        std::chrono::steady_clock::time_point last_access;
        
        CacheEntry(const Value& v) 
            : value(v), frequency(0), last_access(std::chrono::steady_clock::now()) {}
    };

    size_t capacity;
    double alpha;  // Weight for frequency
    double beta;   // Weight for recency
    std::unordered_map<Key, CacheEntry> cache;
    size_t hits;
    size_t misses;
    size_t evictions;

    double calculate_score(const CacheEntry& entry) const {
        auto now = std::chrono::steady_clock::now();
        auto time_since_last_access = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - entry.last_access).count();
        
        // Avoid division by zero
        double recency_score = time_since_last_access > 0 ? 
            1.0 / time_since_last_access : std::numeric_limits<double>::max();
        
        return alpha * entry.frequency + beta * recency_score;
    }

public:
    HybridCache(size_t size, double freq_weight = 0.5, double recency_weight = 0.5)
        : capacity(size), alpha(freq_weight), beta(recency_weight),
          hits(0), misses(0), evictions(0) {}

    bool get(const Key& key, Value& value) {
        auto it = cache.find(key);
        if (it == cache.end()) {
            misses++;
            return false;
        }

        hits++;
        // Update access information
        it->second.frequency++;
        it->second.last_access = std::chrono::steady_clock::now();
        value = it->second.value;
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            // Update existing entry
            it->second.value = value;
            it->second.frequency++;
            it->second.last_access = std::chrono::steady_clock::now();
            return;
        }

        // Check if cache is full
        if (cache.size() >= capacity) {
            // Find entry with lowest score
            auto min_score = std::numeric_limits<double>::max();
            Key evict_key;
            
            for (const auto& entry : cache) {
                double score = calculate_score(entry.second);
                if (score < min_score) {
                    min_score = score;
                    evict_key = entry.first;
                }
            }
            
            cache.erase(evict_key);
            evictions++;
        }

        // Add new entry
        cache.emplace(key, CacheEntry(value));
    }

    size_t size() const {
        return cache.size();
    }

    bool contains(const Key& key) const {
        return cache.find(key) != cache.end();
    }

    void clear() {
        cache.clear();
    }

    // Statistics
    struct CacheStats {
        size_t hits;
        size_t misses;
        size_t evictions;
    };

    CacheStats get_stats() const {
        return CacheStats{hits, misses, evictions};
    }

    double get_hit_ratio() const {
        size_t total = hits + misses;
        return total > 0 ? static_cast<double>(hits) / total : 0.0;
    }
}; 