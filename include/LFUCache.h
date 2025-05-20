#pragma once

#include <unordered_map>
#include <list>
#include <cstddef>

template<typename Key, typename Value>
class LFUCache {
private:
    struct CacheEntry {
        Key key;
        Value value;
        size_t frequency;
        CacheEntry(Key k, Value v) : key(k), value(v), frequency(0) {}
    };

    size_t capacity;
    size_t min_frequency;
    std::unordered_map<Key, typename std::list<CacheEntry>::iterator> cache_map;
    std::unordered_map<size_t, std::list<CacheEntry>> frequency_map;
    size_t hits;
    size_t misses;
    size_t evictions;

public:
    explicit LFUCache(size_t size) 
        : capacity(size), min_frequency(0), hits(0), misses(0), evictions(0) {}

    bool get(const Key& key, Value& value) {
        auto it = cache_map.find(key);
        if (it == cache_map.end()) {
            misses++;
            return false;
        }

        hits++;
        // Update frequency
        auto& entry = it->second;
        size_t old_freq = entry->frequency;
        value = entry->value;

        // Remove from old frequency list
        frequency_map[old_freq].erase(entry);
        if (frequency_map[old_freq].empty()) {
            frequency_map.erase(old_freq);
            if (min_frequency == old_freq) {
                min_frequency++;
            }
        }

        // Add to new frequency list
        entry->frequency++;
        frequency_map[entry->frequency].push_front(*entry);
        cache_map[key] = frequency_map[entry->frequency].begin();

        return true;
    }

    void put(const Key& key, const Value& value) {
        if (capacity == 0) return;

        auto it = cache_map.find(key);
        if (it != cache_map.end()) {
            // Update existing entry
            it->second->value = value;
            Value dummy;
            get(key, dummy); // Update frequency
            return;
        }

        // Check if cache is full
        if (cache_map.size() >= capacity) {
            // Remove least frequently used item
            auto& min_freq_list = frequency_map[min_frequency];
            auto lfu_entry = min_freq_list.back();
            cache_map.erase(lfu_entry.key);
            min_freq_list.pop_back();
            if (min_freq_list.empty()) {
                frequency_map.erase(min_frequency);
            }
            evictions++;
        }

        // Add new entry
        min_frequency = 1;
        frequency_map[1].push_front(CacheEntry(key, value));
        cache_map[key] = frequency_map[1].begin();
    }

    size_t size() const {
        return cache_map.size();
    }

    bool contains(const Key& key) const {
        return cache_map.find(key) != cache_map.end();
    }

    void clear() {
        cache_map.clear();
        frequency_map.clear();
        min_frequency = 0;
    }

    // Statistics
    size_t get_hits() const { return hits; }
    size_t get_misses() const { return misses; }
    size_t get_evictions() const { return evictions; }
    double get_hit_ratio() const {
        size_t total = hits + misses;
        return total > 0 ? static_cast<double>(hits) / total : 0.0;
    }
}; 