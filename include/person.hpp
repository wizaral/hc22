#pragma once

#include <array>
#include <vector>

inline constexpr auto max_person_items = 5;

using ProductsList = std::array<int32_t, max_person_items>;

struct Person {
    ProductsList favorite{-1, -1, -1, -1, -1};
    ProductsList disliked{-1, -1, -1, -1, -1};
};

using Persons = std::vector<Person>;
