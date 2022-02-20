#include <iostream>

#include "crossover.hpp"
#include "filter.hpp"
#include "generator.hpp"
#include "mutation.hpp"
#include "population.hpp"

int main(int ac, char **av) try {
    if (ac == 2) {
        auto p0 = std::make_unique<Population0>();

        p0->set_input_data(av[1])
            .set_crossover(std::make_unique<Crossover0>())
            .set_filter(std::make_unique<Filter0>())
            .set_generator(std::make_unique<Generator0>())
            .set_mutation(std::make_unique<Mutation0>())
            .algorithm(200, 200);
    } else {
        std::cerr << "Usage: ./pizza [file_name]" << std::endl;
    }
} catch (std::exception &ex) {
    std::cerr << ex.what() << std::endl;
} catch (...) {
    std::cerr << "Uncaught exception" << std::endl;
}
