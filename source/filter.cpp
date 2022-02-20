#include "filter.hpp"

void Filter0::filter(Entities &entities) {
    entities.resize(entities.size() / 4);
}

// ================================================================================================================== //
