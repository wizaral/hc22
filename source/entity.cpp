#include <algorithm>

#include "entity.hpp"

Entity::Entity(const Products &products)
: products(products) {}

int Entity::score() const {
    return m_score;
}

void Entity::examine(const Persons &persons) {
    m_score = 0;

    for (auto i : persons) {
        if (std::none_of(i.disliked.begin(), i.disliked.end(), [this](auto val) {
            return val != -1 && *std::next(products.begin, val);
        })) {
            m_score += std::all_of(i.favorite.begin(), i.favorite.end(), [this](auto val) {
                return val == -1 || *std::next(products.begin, val);
            });
        }
    }
}
