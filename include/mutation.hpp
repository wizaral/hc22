#pragma once

#include <any>
#include <chrono>
#include <random>

#include "entity.hpp"

struct Mutation {
    virtual ~Mutation() = default;

    virtual void mutation(Entities::iterator begin, Entities::iterator end, std::any any) = 0;
};

struct Mutation0 : Mutation {
    std::mt19937 mt{std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count())};

    void mutation(Entities::iterator begin, Entities::iterator end, std::any any) override;
};
