#include <iostream>

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#define EXTENSION ".exe"
#else
#define EXTENSION
#endif

#include "crossover.hpp"
#include "generator.hpp"
#include "mutation.hpp"
#include "population.hpp"
#include "profiler.hpp"

int main(int ac, char **av) try {
#if defined(_WIN64) || defined(_WIN32)
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
#endif
    std::ios::sync_with_stdio(false);

    if (ac > 1) {
        LOG_DURATION("All");

        for (int32_t i = 1; i < ac; ++i) {
            LOG_DURATION("End [" + std::to_string(i) + ']');
            std::cout << "Begin [" << av[i] << ']' << std::endl;

            std::make_unique<PopulationMultiCore0>()
                ->set_input_data(av[i])
                .set_crossover(std::make_unique<CrossoverMultiCore0>())
                .set_generator(std::make_unique<Generator0>())
                .set_mutation(std::make_unique<MutationSingleCore1>())
                .algorithm(240'000, 100);
        }
    } else {
        std::cerr << "Usage: ./pizza" EXTENSION " [files ...]" << std::endl;
    }
} catch (std::exception &ex) {
    std::cerr << ex.what() << std::endl;
} catch (...) {
    std::cerr << "Uncaught exception" << std::endl;
}
