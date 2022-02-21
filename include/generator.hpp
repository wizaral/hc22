#pragma once

#include <any>
#include <chrono>
#include <random>

#include "entity.hpp"
#include "memory_pool.hpp"

struct Generator {
    virtual ~Generator() = default;

    virtual Entities generate(MemoryPool<bool> &memory_pool, size_t products_amount, size_t population_amount) = 0;
    virtual void generate(Entities::iterator begin, Entities::iterator end, std::any any) = 0;
};

struct Generator0 : Generator {
    std::mt19937 mt{std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count())};
    std::uniform_int_distribution<> uid{0, 1};

    Entities generate(MemoryPool<bool> &memory_pool, size_t products_amount, size_t population_amount) override;
    void generate(Entities::iterator begin, Entities::iterator end, std::any any) override;
};
