#pragma once
#include "window.hpp"
#include "raylib.h"

// RAII wrapper for frame drawing operations
class Frame {
private:
    engine::window& window;
public:
    Frame() = delete;
    Frame(engine::window& w);
    ~Frame();
};