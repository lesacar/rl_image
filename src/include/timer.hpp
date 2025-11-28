#pragma once
#include <chrono>

namespace engine {
    class timer {
        private:
            std::chrono::high_resolution_clock clock;
            std::chrono::high_resolution_clock::time_point start;
            // how many times has elapsed_ms() been called
            size_t counter;
        public:
            timer();
            void elapsed_ms();
            ~timer();
    };
}
