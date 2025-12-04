#include "raylib.h"
#include "timer.hpp"
#include "window.hpp"

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
}

Frame::~Frame() {
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
    SetTargetFPS(0);

    // engine::working_image img;
    engine::working_image img(window);

    while (!window.should_close()) {
        Frame frame(window);

        if (IsTextureValid(img.get_tex())) {
            DrawTexture(img.get_tex(), 0, 0, WHITE);
        } else { // no image selected


        }
        DrawFPS(0, 0);
    }
    return 0;
}
