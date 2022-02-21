#pragma once

#include <any>
#include <chrono>
#include <random>
#include <thread>

#include "entity.hpp"
#include "synchronized.hpp"

struct Mutation {
    virtual ~Mutation() = default;

    virtual void mutation(Entities::iterator begin, Entities::iterator end, std::any any) = 0;
};

struct MutationSingleCore0 : Mutation {
    std::mt19937 mt{std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count())};

    void mutation(Entities::iterator begin, Entities::iterator end, std::any any) override;
};

struct MutationSingleCore1 : MutationSingleCore0 {
    std::uniform_int_distribution<> uid{0, 1};

    void mutation(Entities::iterator begin, Entities::iterator end, std::any any) override;
};

// ================================================================================================================== //

struct MutationMultiCore0 : Mutation {
    inline static const auto cores = std::thread::hardware_concurrency();
    al::Synchronized<std::mt19937> mt{std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count())};

    void mutation(Entities::iterator begin, Entities::iterator end, std::any any) override;
};
