#include <algorithm>

#include "crossover.hpp"

#define THREAD_POOL_WAIT
#include "thread_pool.hpp"

void CrossoverSingleCore0::crossover(Entities::iterator begin, Entities::iterator end, std::any any) {
    auto half = std::any_cast<size_t>(any) / 2;

    for (auto it = begin; it != end; std::advance(it, 2)) {
        std::swap_ranges(it->products.begin, std::next(it->products.begin, half), std::next(it)->products.begin);
    }
}

// ================================================================================================================== //

#include <iostream>

void CrossoverMultiCore0::crossover(Entities::iterator begin, Entities::iterator end, std::any any) {
    auto pair = std::any_cast<std::pair<size_t, std::reference_wrapper<al::ThreadPool>>>(any);
    auto half = pair.first / 2;
    auto &pool = pair.second.get();
    auto size = std::distance(begin, end);
    auto shift = size / cores;

    for (size_t i = shift; i < size; i += shift) {
        pool.add_task([i, shift, begin, half]() -> void {
            for (auto it = std::next(begin, i), end = std::next(begin, i + shift); it != end; std::advance(it, 2)) {
                std::swap_ranges(
                    it->products.begin, std::next(it->products.begin, half), std::next(it)->products.begin);
            }
        });
    }

    for (auto it = begin, end = std::next(begin, shift); it != end; std::advance(it, 2)) {
        std::swap_ranges(it->products.begin, std::next(it->products.begin, half), std::next(it)->products.begin);
    }

    pool.wait();
}

// ================================================================================================================== //
