#include "timer.hpp"
#include "engine.hpp"
#include <print>

namespace engine {
    timer::timer() : counter(0), start(clock.now()) {}
    timer::timer(bool print_on_deconstruction) : counter(0), start(clock.now()), print_on_deconstruction(print_on_deconstruction) {}
    void timer::elapsed_ms()  {
                auto var = clock.now() - start;
                auto var2 = std::chrono::duration_cast<std::chrono::milliseconds>(var);
                counter++;
                log(log_level::info, "Timer ({}): {}", counter, var2);
            }
    timer::~timer() {
        if (print_on_deconstruction) {
            log(log_level::info, "Timer deconstructed after running for:");
            elapsed_ms();
        }
    }
}
