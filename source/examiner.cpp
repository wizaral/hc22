#include <algorithm>
#include <execution>

#include "examiner.hpp"

#define THREAD_POOL_WAIT
#include "thread_pool.hpp"

void ExaminerSingleCore0::examine(Entities::iterator begin, Entities::iterator end, std::any any) {
    auto pair = std::any_cast<std::pair<std::reference_wrapper<Persons>, std::reference_wrapper<al::ThreadPool>>>(any);
    auto &persons = pair.first.get();

    std::for_each(std::execution::par_unseq, begin, end, [&persons](Entity &ent) {
        ent.examine(persons);
    });
}

// ================================================================================================================== //

void ExaminerMultiCore0::examine(Entities::iterator begin, Entities::iterator end, std::any any) {
    auto pair = std::any_cast<std::pair<std::reference_wrapper<Persons>, std::reference_wrapper<al::ThreadPool>>>(any);
    auto &persons = pair.first.get();
    auto &pool = pair.second.get();
    size_t size = std::distance(begin, end);
    size_t shift = size / cores;

    for (size_t i = shift; i < size; i += shift) {
        pool.add_task([i, shift, begin, pers = std::cref(persons)]() -> void {
            for (auto it = std::next(begin, i), end = std::next(begin, i + shift); it != end; std::advance(it, 1)) {
                it->examine(pers.get());
            }
        });
    }

    for (auto it = begin, end = std::next(begin, shift); it != end; std::advance(it, 1)) {
        it->examine(persons);
    }

    pool.wait();
}

// ================================================================================================================== //
