#pragma once

#include "entity.hpp"

struct Generator {
    virtual ~Generator() = default;

    virtual Entities generate(size_t products_amount, size_t population_amount) = 0;
};

struct Generator0 : Generator {
    Entities generate(size_t products_amount, size_t population_amount) override;
};

// struct Generator1 : Generator {
//     Entities generate(size_t products_amount, size_t population_amount) override;
// };
