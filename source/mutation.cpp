#include <algorithm>

#include "mutation.hpp"

#define THREAD_POOL_WAIT
#include "thread_pool.hpp"

void MutationSingleCore0::mutation(Entities::iterator begin, Entities::iterator end, std::any any) {
    static_cast<void>(any);

    for (auto it = begin; it != end; ++it) {
        std::shuffle(it->products.begin, it->products.end, mt);
    }
}

// ================================================================================================================== //

void MutationSingleCore1::mutation(Entities::iterator begin, Entities::iterator end, std::any any) {
    auto size = std::distance(begin, end);
    std::uniform_int_distribution<> uid_local{0, static_cast<int>(size) - 1 + (begin == end)};
    static_cast<void>(any);

    for (size_t i = 0, amount = size / 1'000; i < amount; ++i) {
        auto it = std::next(begin, uid_local(mt));
        std::shuffle(it->products.begin, it->products.end, mt);
    }
}

// ================================================================================================================== //

void MutationMultiCore0::mutation(Entities::iterator begin, Entities::iterator end, std::any any) {
    auto &pool = std::any_cast<std::reference_wrapper<al::ThreadPool>>(any).get();
    size_t size = std::distance(begin, end);
    size_t shift = size / cores;

    for (size_t i = shift; i < size; i += shift) {
        pool.add_task([i, begin, end, this]() -> void {
            for (auto it = std::next(begin, i); it != end; ++it) {
                std::shuffle(it->products.begin, it->products.end, mt.get().ref());
            }
        });
    }

    for (auto it = begin, end = std::next(begin, shift); it != end; ++it) {
        std::shuffle(it->products.begin, it->products.end, mt.get().ref());
    }

    pool.wait();
}

// ================================================================================================================== //
