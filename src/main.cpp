#include "raylib.h"
#include "timer.hpp"
#include "window.hpp"
#include <algorithm>
#include <raymath.h>

#pragma warning(push, 0)
#pragma warning(disable: 4996 4267 4244 4005 4018 4101 4189 4456 4457 4458 4459 4505 4701 4703)
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#pragma warning(pop)

#include <engine.hpp>

// just an RAII wrapper for some gpu calls you'd want on every frame
class Frame {
private:
    engine::window& window;
public:
    Frame() = delete;
    Frame(engine::window& w);
    ~Frame();
};

Frame::Frame(engine::window& w) : window(w) {
    BeginDrawing();
    ClearBackground(BLACK);
    window.resize_handler();
    window.mouse_pos_current_frame = GetMousePosition();
}

Frame::~Frame() {
    window.mouse_pos_last_frame = window.mouse_pos_current_frame;
    EndDrawing();
}

int main(int argc, const char* argv[]) {
    static_assert(sizeof(engine::vec2<float>) == sizeof(Vector2), "raylib Vector2 is not the same as engine::vec2<float>");
    engine::timer timmy{false};
    SetTraceLogLevel(LOG_WARNING);

    timmy.elapsed_ms();
    engine::window window{"Image Viewer"};
    timmy.elapsed_ms();
    window.append_cli_args(argc, argv);
    // SetTargetFPS(0);

    // engine::working_image img;
    engine::working_image img(window);

    Vector2 halfer = Vector2{2.0f,2.0f};
    Vector2 ws = Vector2{static_cast<float>(window.size.x), static_cast<float>(window.size.y)};

    window.cam.zoom = 1.0f;
    window.cam.target = Vector2Zero();
    window.cam.offset = Vector2Divide(ws, halfer);
    float rot = 0.0f;

    engine::log(engine::log_level::info, "window.cam.rotation: {}", window.cam.rotation);


    while (!window.should_close()) {
        Frame frame(window);
        ws.x = static_cast<float>(window.size.x);
        ws.y = static_cast<float>(window.size.y);

        if (IsTextureValid(img.get_tex())) {
            float mousey_d = GetMouseWheelMoveV().y * 2;
            float sign = (mousey_d >= 0) ? 1.0f : -1.0f;
            float final_mousey = sign * ((mousey_d*mousey_d) * GetFrameTime());
            window.cam.zoom += window.cam.zoom*final_mousey;
            window.cam.zoom = std::clamp(window.cam.zoom, 0.1f, 10.0f);
            BeginMode2D(window.cam);
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                window.cam.target.x += GetMouseDelta().x;
                window.cam.target.y += GetMouseDelta().y;
                engine::log(engine::log_level::info, "{},{}", window.cam.target.x, window.cam.target.y);
            }
            if (mousey_d != 0) {
                engine::log(engine::log_level::info, "{}", window.cam.zoom);
            }
            if (IsKeyDown(KEY_UP)) {
                rot += 1.0f;
                engine::log(engine::log_level::info, "{}", rot);
            }
            if (IsKeyDown(KEY_DOWN)) {
                rot -= 1.0f;
                engine::log(engine::log_level::info, "{}", rot);
            }


            engine::DrawTextureMidpoint(img.get_tex(), Vector2Zero(), rot);
            EndMode2D();
        } else { // no image selected
            // draw some kind of file picker for an image or ask the user to dragndrop
        }
        DrawFPS(0, 0);
    }
    return 0;
}
