#include "raylib.h"
#include <cstdlib>

#pragma warning(push, 0)
#pragma warning(disable: 4996 4267 4244 4005 4018 4101 4189 4456 4457 4458 4459 4505 4701 4703)
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#pragma warning(pop)

#include <engine.hpp>

#include <string>
#include <string_view>



/* 
[[nodiscard("\'Tried to discard GPU image texture\'")]] Texture2D texture_from_img_or_crash(engine::window& w)
{
    Texture2D tex = {0};
    tex = LoadTextureFromImage(image_was_provided(w));
    if (!IsTextureValid(tex)) {
        engine::log(engine::log_level::error, "Couldn't create GPU texture from image, check GPU drivers.");
        std::exit(-1);
    }
    return tex;
}
*/


int main(int argc, const char* argv[]) {
    static_assert(sizeof(engine::vec2<float>) == sizeof(Vector2), "raylib Vector2 is not the same as engine::vec2<float>");

    engine::window window{"Image Viewer"};
    window.append_cli_args(argc, argv);

    // engine::working_image img;
    engine::working_image img(window);

    while (!window.should_close()) {
        BeginDrawing();
        window.resize_handler();
        ClearBackground(BLACK);

        DrawTexture(img.get_tex(), 0, 0, WHITE);
        
        EndDrawing();
    }
    
    return 0;
}
