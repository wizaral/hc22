#pragma once

#include <any>
#include <thread>

#include "entity.hpp"

struct Crossover {
    virtual ~Crossover() = default;

    virtual void crossover(Entities::iterator begin, Entities::iterator end, std::any any) = 0;
};

struct CrossoverSingleCore0 : Crossover {
    void crossover(Entities::iterator begin, Entities::iterator end, std::any any) override;
};

struct CrossoverMultiCore0 : Crossover {
    inline static const auto cores = std::thread::hardware_concurrency();

    void crossover(Entities::iterator begin, Entities::iterator end, std::any any) override;
};
