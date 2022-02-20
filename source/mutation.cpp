#include <algorithm>

#include "mutation.hpp"

Entities Mutation0::mutation(const Entities &entities) {
    Entities copy = entities;

    for (auto &i : copy) {
        std::shuffle(i.products.begin(), i.products.end(), mt);
    }
    return copy;
}

// ================================================================================================================== //
