#include <algorithm>

#include "mutation.hpp"

void Mutation0::mutation(Entities &entities) {
    entities.insert(entities.end(), entities.begin(), entities.end());

    for (auto it = std::next(entities.begin(), entities.size() / 2), end = entities.end(); it != end; ++it) {
        std::shuffle(it->products.begin(), it->products.end(), mt);
    }
}

Entities Mutation0::mutation(const Entities &entities) {
    Entities copy = entities;

    for (auto &i : copy) {
        std::shuffle(i.products.begin(), i.products.end(), mt);
    }
    return copy;
}

// ================================================================================================================== //
