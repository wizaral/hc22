#include <algorithm>

#include "entity.hpp"

Entity::Entity(const Products &products)
: products(products) {}

uint64_t Entity::score() const {
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

void swap(Entity &a, Entity &b) {
    std::swap_ranges(a.products.begin, a.products.end, b.products.begin);
    std::swap(a.m_score, b.m_score);
}
