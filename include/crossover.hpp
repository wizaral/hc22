#pragma once

#include <any>

#include "entity.hpp"

struct Crossover {
    virtual ~Crossover() = default;

    virtual void crossover(Entities::iterator begin, Entities::iterator end, std::any any) = 0;
};

struct Crossover0 : Crossover {
    void crossover(Entities::iterator begin, Entities::iterator end, std::any any) override;
};
