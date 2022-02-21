#include <fstream>
#include <iostream>
#include <numeric>

#include "population.hpp"

#include "crossover.hpp"
#include "generator.hpp"
#include "mutation.hpp"
#include "profiler.hpp"

Entities &Population::entities() {
    return m_entities;
}

const Entities &Population::entities() const {
    return m_entities;
}

Population &Population::set_input_data(const char *file) {
    m_file = generate_output_file_name(file);

    if (std::ifstream ifile(file); ifile) {
        size_t amount, id = 0;

        ifile >> amount;
        m_persons.resize(amount);

        for (auto &person : m_persons) {
            read_products(ifile, person.favorite, id);
            read_products(ifile, person.disliked, id);
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

void Population::examine() {
    auto size = m_entities.size();
    auto shift = size / cores;

    for (size_t i = shift; i < size; i += shift) {
        m_thread_pool.add_task([i, shift, pers = std::cref(m_persons), ents = std::ref(m_entities)]() -> void {
            for (auto j = i, end = i + shift; j < end; ++j) {
                ents.get()[j].examine(pers);
            }
        });
    }

    for (size_t i = 0; i < shift; ++i) {
        m_entities[i].examine(m_persons);
    }

    m_thread_pool.wait();
}

void Population::print(const Entity &entity) {
    std::cout << "Score: " << entity.score() << '\n';

    if (std::ofstream ofile(m_file); ofile) {
        ofile << std::accumulate(entity.products.begin, entity.products.end, 0);

        for (size_t i = 0, size = m_products.size(); i < size; ++i) {
            if (*std::next(entity.products.begin, i)) {
                ofile << ' ' << m_products[i];
            }
        }
    }
}

void Population::read_products(std::ifstream &file, std::array<int64_t, max_person_items> &products_list, size_t &id) {
    std::string str;
    size_t amount;
    file >> amount;

    for (size_t i = 0; i < amount; ++i) {
        file >> str;

        auto [it, res] = m_products_ids.insert({str, id});
        if (res) {
            products_list[i] = id;
            m_products.emplace_back(it->first);
            ++id;
        } else {
            products_list[i] = it->second;
        }
    }
}

void Population::sort(Entities &entities) {
    std::sort(entities.begin(), entities.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.score() > rhs.score();
    });
}

std::string Population::generate_output_file_name(const char *name) {
    std::string file(name);

    if (auto pos = file.rfind('.'); pos != std::string::npos) {
        file.replace(std::next(file.begin(), pos), file.end(), ".out");
    } else {
        file.append("_out");
    }

    if (auto pos = file.rfind("input/"); pos != std::string::npos) {
        file.replace(std::next(file.begin(), pos), std::next(file.begin(), pos + 2), "out");
    }
    return file;
}

// ================================================================================================================== //

void Population0::algorithm(size_t population_amount, size_t iterations) {
    m_memory_pool.pool.resize(m_products.size() * population_amount);
    m_memory_pool.optional_size = population_amount;

    m_entities = m_generator->generate(m_memory_pool, m_products.size(), population_amount);

    std::cout << "Start" << std::endl;

    for (size_t i = 0; i < iterations; ++i, ++m_age) {
        examine();
        sort(m_entities);

        std::cout << "Age: " << m_age << ". Best: " << m_entities.front().score() << '\n';

        {
            LOG_DURATION("Crossover");

            m_crossover->crossover(
                std::next(m_entities.begin(), m_entities.size() / 4),
                std::next(m_entities.begin(), m_entities.size() / 2),
                {m_products.size() / 2}
            );
        }

        {
            LOG_DURATION("Mutation");


            m_mutation->mutation(
                std::next(m_entities.begin(), m_entities.size() / 2),
                m_entities.end(),
                {});
        }
    }

    print(m_entities.front());
}

// ================================================================================================================== //
