#include <chrono>
#include <random>

#include "generator.hpp"

Entities Generator0::generate(size_t products_amount, size_t population_amount) {
    std::mt19937 mt(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> uid(0, 1);

    Entities entities;
    entities.reserve(population_amount);

    for (size_t i = 0; i < population_amount; ++i) {
        Entity entity(products_amount);

        for (size_t j = 0; j < products_amount; ++j) {
            entity.products[j] = uid(mt);
        }

        entities.emplace_back(entity);
    }
    return entities;
}

// ================================================================================================================== //
