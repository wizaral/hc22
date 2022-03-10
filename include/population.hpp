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

    std::unique_ptr<struct Crossover> m_crossover;
    std::unique_ptr<struct Examiner> m_examiner;
    std::unique_ptr<struct Generator> m_generator;
    std::unique_ptr<struct Mutation> m_mutation;

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

    Population &set_crossover(std::unique_ptr<struct Crossover> crossover);
    Population &set_examiner(std::unique_ptr<struct Examiner> examiner);
    Population &set_generator(std::unique_ptr<struct Generator> generator);
    Population &set_mutation(std::unique_ptr<struct Mutation> mutation);

    virtual void algorithm(size_t population_amount, size_t iterations) = 0;

protected:
    void print(const Entity &entity);
    void read_products(std::ifstream &file, ProductsList &products_list, int32_t &id);

    static void sort(Entities &entities);
    static std::string generate_output_file_name(const char *name);
};

class Population0 : public Population {
public:
    void algorithm(size_t population_amount, size_t iterations) override;
};

class Population1 : public Population {
protected:
    enum class Product : uint8_t {
        Favorite = 0x1 << 0,
        Disliked = 0x1 << 1,
    };

    std::vector<Product> m_products_state;

public:
    void algorithm(size_t population_amount, size_t iterations) override;

protected:
    void analyze_products();
    std::vector<bool> get_adam();
};
