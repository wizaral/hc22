#pragma once

#include <vector>

#include "person.hpp"

struct Products {
    std::vector<bool>::iterator begin;
    std::vector<bool>::iterator end;
};

class Entity {
    uint64_t m_score = 0;

public:
    Products products;

    Entity() = default;
    Entity(const Products &products);

    uint64_t score() const;
    void examine(const Persons &persons);

    friend void swap(Entity &a, Entity &b);
};

using Entities = std::vector<Entity>;
