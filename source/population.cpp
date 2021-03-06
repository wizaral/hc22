#include <execution>
#include <fstream>
#include <iostream>
#include <numeric>

#include "population.hpp"

#include "crossover.hpp"
#include "examiner.hpp"
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
        size_t amount;
        int32_t id = 0;

        ifile >> amount;
        m_persons.resize(amount);

        for (auto &person : m_persons) {
            read_products(ifile, person.favorite, id);
            read_products(ifile, person.disliked, id);
        }
#if 0
        for (auto i : m_products) {
            std::cout << i << ' ';
        }
        std::cout << '\n';

        for (auto &[k, v] : m_products_ids) {
            std::cout << k << ':' << v << ' ';
        }
        std::cout << '\n';

        for (auto &i : m_persons) {
            for (auto j : i.favorite) {
                std::cout << m_products[j] << ' ';
            }
            std::cout << '\n';

            for (auto j : i.disliked) {
                std::cout << m_products[j] << ' ';
            }
            std::cout << '\n';
        }
#endif
    } else {
        throw std::invalid_argument("Wrong file name");
    }

    return *this;
}

Population &Population::set_crossover(std::unique_ptr<Crossover> crossover) {
    m_crossover = std::move(crossover);
    return *this;
}

Population &Population::set_examiner(std::unique_ptr<Examiner> examiner) {
    m_examiner = std::move(examiner);
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

void Population::read_products(std::ifstream &file, ProductsList &products_list, int32_t &id) {
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
    std::sort(std::execution::par_unseq, entities.begin(), entities.end(), [](const auto &lhs, const auto &rhs) {
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
        m_examiner->examine(
            m_entities.begin(),
            m_entities.end(),
            std::pair{std::ref(m_persons), std::ref(m_thread_pool)}
        );

        sort(m_entities);

        std::cout << "Age: " << m_age << ". Best: " << m_entities.front().score() << '\n';

        {
            LOG_DURATION("Crossover");

            std::copy(
                m_memory_pool.pool.begin(),
                std::next(m_memory_pool.pool.begin(), m_memory_pool.pool.size() / 4),
                std::next(m_memory_pool.pool.begin(), m_memory_pool.pool.size() / 4)
            );

            m_crossover->crossover(
                std::next(m_entities.begin(), m_entities.size() / 4),
                std::next(m_entities.begin(), m_entities.size() / 2),
                {std::pair{m_products.size(), std::ref(m_thread_pool)}}
            );
        }

        {
            LOG_DURATION("Mutation");

            std::copy(
                m_memory_pool.pool.begin(),
                std::next(m_memory_pool.pool.begin(), m_memory_pool.pool.size() / 2),
                std::next(m_memory_pool.pool.begin(), m_memory_pool.pool.size() / 2)
            );

            m_mutation->mutation(
                std::next(m_entities.begin(), m_entities.size() / 2),
                m_entities.end(),
                {std::ref(m_thread_pool)});
        }
    }

    print(m_entities.front());
}

// ================================================================================================================== //

void Population1::algorithm(size_t population_amount, size_t iterations) {
    m_memory_pool.pool.resize(m_products.size() * population_amount);
    m_memory_pool.optional_size = population_amount;

    m_entities = m_generator->generate(m_memory_pool, m_products.size(), population_amount);

    auto adam = get_adam();
    std::copy(adam.begin(), adam.end(), m_memory_pool.pool.begin());

    std::cout << "Start" << std::endl;

    for (size_t i = 0; i < iterations; ++i, ++m_age) {
        m_examiner->examine(
            m_entities.begin(),
            m_entities.end(),
            std::pair{std::ref(m_persons), std::ref(m_thread_pool)}
        );

        sort(m_entities);

        std::cout << "Age: " << m_age << ". Best: " << m_entities.front().score() << '\n';

        {
            LOG_DURATION("Crossover");

            std::copy(
                m_memory_pool.pool.begin(),
                std::next(m_memory_pool.pool.begin(), m_memory_pool.pool.size() / 4),
                std::next(m_memory_pool.pool.begin(), m_memory_pool.pool.size() / 4)
            );

            m_crossover->crossover(
                std::next(m_entities.begin(), m_entities.size() / 4),
                std::next(m_entities.begin(), m_entities.size() / 2),
                {std::pair{m_products.size(), std::ref(m_thread_pool)}}
            );
        }

        {
            LOG_DURATION("Mutation");

            std::copy(
                m_memory_pool.pool.begin(),
                std::next(m_memory_pool.pool.begin(), m_memory_pool.pool.size() / 2),
                std::next(m_memory_pool.pool.begin(), m_memory_pool.pool.size() / 2)
            );

            m_mutation->mutation(
                std::next(m_entities.begin(), m_entities.size() / 2),
                m_entities.end(),
                {std::ref(m_thread_pool)});
        }
    }

    print(m_entities.front());
}

void Population1::analyze_products() {
    for (auto &p : m_persons) {
        for (auto i : p.favorite) {
            auto val = static_cast<uint8_t>(m_products_state[i]);
            m_products_state[i] = static_cast<Product>(val |= static_cast<uint8_t>(Product::Favorite));
        }
        for (auto i : p.disliked) {
            auto val = static_cast<uint8_t>(m_products_state[i]);
            m_products_state[i] = static_cast<Product>(val |= static_cast<uint8_t>(Product::Disliked));
        }
    }
}

std::vector<bool> Population1::get_adam() {
    std::vector<bool> adam(m_products.size(), true);

    m_products_state.resize(m_products.size());
    analyze_products();

    for (size_t i = 0, size = m_products_state.size(); i < size; ++i) {
        if ((static_cast<uint8_t>(m_products_state[i]) & static_cast<uint8_t>(Product::Disliked))) {
            adam[i] = false;
        }
    }
    return adam;
}

// ================================================================================================================== //
