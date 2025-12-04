#include <engine.hpp>
#include <raylib.h>
#include <string_view>
#include <raymath.h>

namespace engine {
    bool is_supported_image_extension(std::string_view img_path) {
        if (img_path.empty()) return false;
        for (auto ft : global::image_filetypes) {
            if (IsFileExtension(img_path.data(), ft.data())) {
                return true;
            }
        }
        return false;
    }

    Image image_was_provided(window& w)
    {
        Image img = {};
        if (w.get_cli_args().size() > 1) {
            img = LoadImage(w.get_cli_args().at(1).data());
        }

        if (!IsImageValid(img)) {
            engine::log(engine::log_level::info, "No image provided from commandline");
            UnloadImage(img);
            img = {};
        }

        return img;
    }

    void DrawTextureMidpoint(Texture2D tex, Vector2 point, float rotation) {
        Rectangle texture = Rectangle{0.0f, 0.0f, static_cast<float>(tex.width), static_cast<float>(tex.height)};
        Rectangle dst = Rectangle{point.x-tex.width/2.0f, point.y-tex.height/2.0f, static_cast<float>(tex.width), static_cast<float>(tex.height)};
        DrawTexturePro(tex, texture, dst, Vector2Zero(), rotation, WHITE);
    }

}
