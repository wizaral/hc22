#pragma once

#include <array>
#include <vector>

inline constexpr auto max_person_items = 5;

struct Person {
    std::array<int64_t, max_person_items> favorite{-1, -1, -1, -1, -1};
    std::array<int64_t, max_person_items> disliked{-1, -1, -1, -1, -1};
};

using Persons = std::vector<Person>;
