#include "raylib.h"
#include <engine.hpp>
#include <string>
#include <string_view>

int main(int argc, const char* argv[]) {
    static_assert(sizeof(engine::vec2<float>) == sizeof(Vector2), "raylib Vector2 is not the same as engine::vec2<float>");
    engine::window window{"Image Viewer"};
    window.append_cli_args(argc, argv);

    Image img;

    if (window.get_cli_args().size() > 1) {
    
        img = LoadImage(window.get_cli_args().at(1).data());
    }

    if (!IsImageValid(img)) {
        engine::log(engine::log_level::error, "No valid image provided");
        std::abort();
    }

    engine::log(engine::log_level::info, "Image {} loaded: ({}x{})", window.get_cli_args().at(1), img.width, img.height);

    Texture2D tex = LoadTextureFromImage(img);
    if (!IsTextureValid(tex)) {
        engine::log(engine::log_level::error, "No valid image provided");
        std::abort();
    }

    while (!window.should_close()) {
        BeginDrawing();
        window.resize_handler();
       
        ClearBackground(BLACK);
        std::string string_to_draw = "This is a centered test string";
        // default raylib font needs spacing of 2, and DrawText() which uses the default font implicitly has spacing of fontsize/10
        Vector2 string_dim = MeasureTextEx(GetFontDefault(), string_to_draw.c_str(), 20, 2);

        DrawTextEx(GetFontDefault(), string_to_draw.c_str(), Vector2{(window.size.x - string_dim.x)/2, (window.size.y - string_dim.y)/2}, 20, 2, RAYWHITE);

        DrawTexture(tex, 0, 0, WHITE);
        
        EndDrawing();
        if (WindowShouldClose()) {
            window.close();
            // engine::log(engine::log_level::info, "windowShouldClose: {}", global::windowShouldClose);
        }
    }
    
    return 0;
}
