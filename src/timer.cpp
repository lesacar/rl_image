#include "timer.hpp"
#include "engine.hpp"

namespace engine {
    timer::timer() : counter(0) {}
    void timer::elapsed_ms()  {
                auto var = clock.now() - start;
                auto var2 = std::chrono::duration_cast<std::chrono::milliseconds>(var);
                counter++;
                log(log_level::info, "Timer ({}): {}", counter, var2);
            }
    timer::~timer() {}
}
