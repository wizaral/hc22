#include <algorithm>

#include "crossover.hpp"

void Crossover0::crossover(Entities::iterator begin, Entities::iterator end, std::any any) {
    auto half = std::any_cast<size_t>(any) / 2;

    for (auto it = begin; it != end; std::advance(it, 2)) {
        std::swap_ranges(it->products.begin, std::next(it->products.begin, half), std::next(it)->products.begin);
    }
}

// ================================================================================================================== //
