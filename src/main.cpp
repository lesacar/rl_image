#include "raylib.h"
#include "timer.hpp"

#pragma warning(push, 0)
#pragma warning(disable: 4996 4267 4244 4005 4018 4101 4189 4456 4457 4458 4459 4505 4701 4703)
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#pragma warning(pop)

#include <engine.hpp>

int main(int argc, const char* argv[]) {
    static_assert(sizeof(engine::vec2<float>) == sizeof(Vector2), "raylib Vector2 is not the same as engine::vec2<float>");
    engine::timer timmy{true};
    SetTraceLogLevel(LOG_WARNING);

    timmy.elapsed_ms();
    engine::window window{"Image Viewer"};
    timmy.elapsed_ms();
    window.append_cli_args(argc, argv);
    SetTargetFPS(0);

    // engine::working_image img;
    engine::working_image img(window);

    while (!window.should_close()) {
        BeginDrawing();
        window.resize_handler();
        ClearBackground(BLACK);

        DrawTexture(img.get_tex(), 0, 0, WHITE);
        DrawFPS(0, 0);

        
        EndDrawing();
    }
    
    return 0;
}
