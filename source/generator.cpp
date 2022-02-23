#include "generator.hpp"

Entities Generator0::generate(MemoryPool<bool> &memory_pool, size_t products_amount, size_t population_amount) {
    Entities entities;
    entities.reserve(population_amount);

    auto it = memory_pool.pool.begin();
    for (size_t i = 0; i < population_amount; ++i) {
        Entity entity({it, std::next(it, products_amount)});

        for (size_t j = 0; j < products_amount; ++j) {
            *std::next(entity.products.begin, j) = uid(mt);
        }

        entities.emplace_back(entity);
        std::advance(it, products_amount);
    }
    return entities;
}

void Generator0::generate(Entities::iterator begin, Entities::iterator end, std::any any) {
    auto products_amount = std::any_cast<size_t>(any);

    for (auto it = begin; it != end; ++it) {
        for (size_t j = 0; j < products_amount; ++j) {
            *std::next(it->products.begin, j) = uid(mt);
        }
    }
}

// ================================================================================================================== //
