#include <fstream>
#include <iostream>
#include <numeric>

#include "population.hpp"

#include "crossover.hpp"
#include "filter.hpp"
#include "generator.hpp"
#include "mutation.hpp"

Entities &Population::entities() {
    return m_entities;
}

const Entities &Population::entities() const {
    return m_entities;
}

Population &Population::set_input_data(const char *file) {
    m_file = file;

    if (auto pos = m_file.rfind('.'); pos != std::string::npos) {
        m_file.replace(std::next(m_file.begin(), pos), m_file.end(), ".out");
    } else {
        m_file.append("_out");
    }

    if (auto pos = m_file.rfind("input/"); pos != std::string::npos) {
        m_file.replace(std::next(m_file.begin(), pos), std::next(m_file.begin(), pos + 2), "out");
    }

    if (std::ifstream ifile(file); ifile) {
        size_t amount, id = 0;
        std::string str;

        ifile >> amount;
        m_persons.resize(amount);

        for (auto &person : m_persons) {
            ifile >> amount;

            for (size_t i = 0; i < amount; ++i) {
                ifile >> str;

                auto [it, res] = m_products_ids.insert({str, id});
                if (res) {
                    person.favorite[i] = id;
                    m_products.emplace_back(it->first);
                    ++id;
                } else {
                    person.favorite[i] = it->second;
                }
            }

            ifile >> amount;

            for (size_t i = 0; i < amount; ++i) {
                ifile >> str;

                auto [it, res] = m_products_ids.insert({str, id});
                if (res) {
                    person.disliked[i] = id;
                    m_products.emplace_back(it->first);
                    ++id;
                } else {
                    person.disliked[i] = it->second;
                }
            }
        }
    } else {
        throw std::invalid_argument("Wrong file name");
    }

    return *this;
}

Population &Population::set_crossover(std::unique_ptr<Crossover> crossover) {
    m_crossover = std::move(crossover);
    return *this;
}

Population &Population::set_generator(std::unique_ptr<Generator> generator) {
    m_generator = std::move(generator);
    return *this;
}

Population &Population::set_mutation(std::unique_ptr<Mutation> mutation) {
    m_mutation = std::move(mutation);
    return *this;
}

Population &Population::set_filter(std::unique_ptr<Filter> filter) {
    m_filter = std::move(filter);
    return *this;
}

void Population::examine() {
    auto size = m_entities.size();
    auto shift = size / cores;

    for (size_t i = shift; i < size; i += shift) {
        m_pool.add_task([i, shift, pers = std::ref(m_persons), ents = std::ref(m_entities)]() -> void {
            for (auto j = i, end = i + shift; j < end; ++j) {
                ents.get()[j].examine(pers);
            }
        });
    }

    for (size_t i = 0; i < shift; ++i) {
        m_entities[i].examine(m_persons);
    }

    m_pool.wait();
}

void Population::sort(Entities &entities) {
    std::sort(
        entities.begin(), entities.end(), [](const auto &lhs, const auto &rhs) { return lhs.score() > rhs.score(); });
}

void Population::print(const Entity &entity) {
    std::cout << "Score: " << entity.score() << '\n';

    if (std::ofstream ofile(m_file); ofile) {
        ofile << std::accumulate(entity.products.begin(), entity.products.end(), 0);

        for (size_t i = 0, size = m_products.size(); i < size; ++i) {
            if (entity.products[i]) {
                ofile << ' ' << m_products[i];
            }
        }
    }
}

// ================================================================================================================== //

void Population0::algorithm(size_t population_amount, size_t iterations) {
    m_entities = m_generator->generate(m_products.size(), population_amount);

    for (size_t i = 0; i < iterations; ++i, ++m_age) {
        examine();
        sort(m_entities);

        std::cout << "Best: " << m_entities.front().score() << '\n';
        m_filter->filter(m_entities);

        auto temp0 = m_crossover->crossover(m_entities);
        m_entities.insert(
            m_entities.end(), std::make_move_iterator(temp0.begin()), std::make_move_iterator(temp0.end()));

        auto temp1 = m_mutation->mutation(m_entities);
        m_entities.insert(
            m_entities.end(), std::make_move_iterator(temp1.begin()), std::make_move_iterator(temp1.end()));
    }

    print(m_entities.front());
}

// ================================================================================================================== //
