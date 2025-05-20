#pragma once

#include <list>
#include <unordered_map>
#include <cstddef>
#include <stdexcept>

template<typename Key, typename Value>
class LRUCache {
private:
    struct CacheEntry {
        Key key;
        Value value;
        CacheEntry(Key k, Value v) : key(k), value(v) {}
    };

    size_t capacity;
    std::list<CacheEntry> cache_list;
    std::unordered_map<Key, typename std::list<CacheEntry>::iterator> cache_map;
    size_t hits;
    size_t misses;
    size_t evictions;

public:
    explicit LRUCache(size_t size) 
        : capacity(size), hits(0), misses(0), evictions(0) {}

    bool get(const Key& key, Value& value) {
        auto it = cache_map.find(key);
        if (it == cache_map.end()) {
            misses++;
            return false;
        }

        hits++;
        // Move accessed item to front (most recently used)
        cache_list.splice(cache_list.begin(), cache_list, it->second);
        value = it->second->value;
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto it = cache_map.find(key);
        if (it != cache_map.end()) {
            // Update existing entry
            cache_list.splice(cache_list.begin(), cache_list, it->second);
            it->second->value = value;
            return;
        }

        // Check if cache is full
        if (cache_map.size() >= capacity) {
            // Remove least recently used item
            cache_map.erase(cache_list.back().key);
            cache_list.pop_back();
            evictions++;
        }

        // Add new entry
        cache_list.emplace_front(key, value);
        cache_map[key] = cache_list.begin();
    }

    size_t size() const {
        return cache_map.size();
    }

    bool contains(const Key& key) const {
        return cache_map.find(key) != cache_map.end();
    }

    void clear() {
        cache_list.clear();
        cache_map.clear();
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