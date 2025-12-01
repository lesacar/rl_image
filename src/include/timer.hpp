#pragma once
#include <chrono>

namespace engine {
    class timer {
        private:
            std::chrono::high_resolution_clock clock;
            std::chrono::high_resolution_clock::time_point start;
            bool print_on_deconstruction = false;
            // how many times has elapsed_ms() been called
            size_t counter;
        public:
            timer();
            timer(bool print_on_deconstruction);
            void elapsed_ms();
            ~timer();
    };
}
