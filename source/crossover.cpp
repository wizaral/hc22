#include <algorithm>

#include "crossover.hpp"

Entities Crossover0::crossover(const Entities &entities) {
    Entities copy = entities;
    auto size = copy.front().products.size() / 2;

    for (size_t i = 0, j = copy.size(); i < j; i += 2) {
        auto it = copy[i].products.begin();

        std::swap_ranges(it, std::next(it, size), copy[i + 1].products.begin());
    }
    return copy;
}

// ================================================================================================================== //
