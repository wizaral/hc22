#pragma once

#include "entity.hpp"

struct Filter {
    virtual ~Filter() = default;

    virtual void filter(Entities &entities) = 0;
};

struct Filter0 : Filter {
    void filter(Entities &entities) override;
};

// struct Filter1 : Filter {
//     void filter(Entities &entities) override;
// };
