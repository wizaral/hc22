#include <algorithm>

#include "crossover.hpp"

void Crossover0::crossover(Entities &entities) {
    auto half = entities.front().products.size() / 2;
    entities.insert(entities.end(), entities.begin(), entities.end());

    for (auto it = std::next(entities.begin(), entities.size() / 2), end = entities.end(); it != end; std::advance(it, 2)) {
        auto _ = it->products.begin();
        std::swap_ranges(_, std::next(_, half), std::next(it)->products.begin());
    }
}

Entities Crossover0::crossover(const Entities &entities) {
    Entities copy = entities;
    auto half = copy.front().products.size() / 2;

    for (size_t i = 0, size = copy.size(); i < size; i += 2) {
        auto it = copy[i].products.begin();
        std::swap_ranges(it, std::next(it, half), copy[i + 1].products.begin());
    }
    return copy;
}

// ================================================================================================================== //
