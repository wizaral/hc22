#pragma once

#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "entity.hpp"
#include "memory_pool.hpp"

#define THREAD_POOL_WAIT
#include "thread_pool.hpp"

class Population {
protected:
    Entities m_entities;
    uint64_t m_age = 0;

    std::unique_ptr<class Crossover> m_crossover;
    std::unique_ptr<class Generator> m_generator;
    std::unique_ptr<class Mutation> m_mutation;

    std::string m_file;

    Persons m_persons;
    MemoryPool<bool> m_memory_pool;

    std::vector<std::string_view> m_products;
    std::map<std::string, int32_t> m_products_ids;

    al::ThreadPool m_thread_pool{cores - 1};
    inline static const auto cores = std::thread::hardware_concurrency();

public:
    virtual ~Population() = default;

    Entities &entities();
    const Entities &entities() const;

    Population &set_input_data(const char *file);

    Population &set_crossover(std::unique_ptr<Crossover> crossover);
    Population &set_generator(std::unique_ptr<Generator> generator);
    Population &set_mutation(std::unique_ptr<Mutation> mutation);

    virtual void algorithm(size_t population_amount, size_t iterations) = 0;

protected:
    virtual void examine() = 0;
    void print(const Entity &entity);
    void read_products(std::ifstream &file, std::array<int64_t, max_person_items> &products_list, size_t &id);

    static void sort(Entities &entities);
    static std::string generate_output_file_name(const char *name);
};

class PopulationSingleCore0 : public Population {
public:
    void algorithm(size_t population_amount, size_t iterations) override;
    void examine() override;
};

class PopulationMultiCore0 : public Population {
public:
    void algorithm(size_t population_amount, size_t iterations) override;
    void examine() override;
};
