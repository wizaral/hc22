#include <algorithm>

#include "mutation.hpp"

void Mutation0::mutation(Entities::iterator begin, Entities::iterator end, std::any any) {
    static_cast<void>(any);

    for (auto it = begin; it != end; ++it) {
        std::shuffle(it->products.begin, it->products.end, mt);
    }
}

// ================================================================================================================== //
