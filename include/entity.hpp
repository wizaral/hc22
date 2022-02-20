#pragma once

#include <vector>

#include "person.hpp"

using Products = std::vector<bool>;

class Entity {
    uint64_t m_score = 0;

public:
    Products products;

    Entity() = default;
    Entity(size_t products_amount);

    int score() const;
    void examine(const Persons &persons);
};

using Entities = std::vector<Entity>;
