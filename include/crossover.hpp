#pragma once

#include "entity.hpp"

struct Crossover {
    virtual ~Crossover() = default;

    virtual Entities crossover(const Entities &entities) = 0;
};

struct Crossover0 : Crossover {
    Entities crossover(const Entities &entities) override;
};

// struct Crossover1 : Crossover {
//     Entities crossover(const Entities &entities) override;
// };
