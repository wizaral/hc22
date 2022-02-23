#pragma once

#include <any>
#include <thread>

#include "entity.hpp"

struct Examiner {
    virtual ~Examiner() = default;

    virtual void examine(Entities::iterator begin, Entities::iterator end, std::any any) = 0;
};

struct ExaminerSingleCore0 : Examiner {
    void examine(Entities::iterator begin, Entities::iterator end, std::any any) override;
};

struct ExaminerMultiCore0 : Examiner {
    inline static const auto cores = std::thread::hardware_concurrency();

    void examine(Entities::iterator begin, Entities::iterator end, std::any any) override;
};
