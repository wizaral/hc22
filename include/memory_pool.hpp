#pragma once

#include <vector>

template <class T>
struct MemoryPool final {
    std::vector<T> pool;
    size_t optional_size = 0;


    MemoryPool() = default;

    explicit MemoryPool(size_t size)
    : pool(size)
    , optional_size(0) {}

    MemoryPool(const MemoryPool &) = delete;
    MemoryPool(MemoryPool &&) = delete;
    MemoryPool &operator=(const MemoryPool &) = delete;
    MemoryPool &operator=(MemoryPool &&) = delete;

    ~MemoryPool() = default;
};
