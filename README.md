# Hybrid LRU + LFU Cache Mapping System

A C++ implementation of a hybrid cache mapping system that combines Least Recently Used (LRU) and Least Frequently Used (LFU) caching mechanisms. This project is designed for educational purposes in Operating Systems courses.

## Features

- Implements both LRU and LFU caching mechanisms
- Combines them using a hybrid eviction policy
- Simulates multiple processes accessing memory pages
- Provides real-time visualization of cache activity
- Tracks and displays cache statistics (hit ratio, evictions, etc.)
- Interactive test mode for predicting evictions

## Building the Project

### Prerequisites

- C++17 compatible compiler
- CMake 3.10 or higher
- Make or your preferred build system

### Build Instructions

```bash
# Create a build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
make
```

## Usage

After building, run the simulator:

```bash
./cache_simulator [options]
```

### Command Line Options

- `-s, --size <size>`: Cache size (default: 4)
- `-a, --alpha <weight>`: Frequency weight (default: 0.5)
- `-b, --beta <weight>`: Recency weight (default: 0.5)
- `-n, --num-accesses <n>`: Number of accesses (default: 20)
- `-i, --interactive`: Enable interactive test mode
- `-h, --help`: Show help message

### Example

```bash
# Run with default settings
./cache_simulator

# Run with custom cache size and weights
./cache_simulator -s 8 -a 0.7 -b 0.3

# Run in interactive mode
./cache_simulator -i
```

## Implementation Details

### Cache Components

1. **LRU Cache**
   - Uses a doubly-linked list for O(1) access and updates
   - Maintains recency information

2. **LFU Cache**
   - Tracks frequency of page accesses
   - Uses frequency maps for efficient updates

3. **Hybrid Cache**
   - Combines LRU and LFU using weighted scoring
   - Score = α * frequency + β * (1 / time_since_last_use)
   - Configurable weights (α and β) for tuning

### Performance Considerations

- O(1) time complexity for cache operations
- Efficient memory usage with STL containers
- Thread-safe operations for multi-process simulation

## Project Structure

```
HybridCacheSimulator/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── LRUCache.h
│   ├── LFUCache.h
│   ├── HybridCache.h
│   └── CacheSimulator.h
├── src/
│   ├── main.cpp
│   └── CacheSimulator.cpp
└── data/
    └── sample_input.txt
```

## Contributing

Feel free to submit issues and enhancement requests!

## License

This project is licensed under the MIT License - see the LICENSE file for details. 