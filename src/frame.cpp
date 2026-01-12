#include "frame.hpp"
#include "engine.hpp"

Frame::Frame(engine::window& w) : window(w) {
    BeginDrawing();
    ClearBackground(BLACK);
    window.resize_handler();
    window.mouse_pos_current_frame = GetMousePosition();
}

Frame::~Frame() {
    window.mouse_pos_last_frame = window.mouse_pos_current_frame;
    if (window.show_fps) {
        DrawFPS(0, 0);
    }
    EndDrawing();
}