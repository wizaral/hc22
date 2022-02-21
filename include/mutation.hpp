#pragma once

#include <chrono>
#include <random>

#include "entity.hpp"

struct Mutation {
    virtual ~Mutation() = default;

    virtual void mutation(Entities &entities) = 0;
    virtual Entities mutation(const Entities &entities) = 0;
};

struct Mutation0 : Mutation {
    std::mt19937 mt{std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count())};

    void mutation(Entities &entities) override;
    Entities mutation(const Entities &entities) override;
};

// struct Mutation1 : Mutation0 {
//     Entities mutation(const Entities &entities) override;
// };
