#include <cmath>
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
            const char* imagepathfromcmd = w.get_cli_args().at(1).data();
            // raylib default load
            img = LoadImage(imagepathfromcmd);
            if (!IsImageValid(img)) { // try secondary custom load
                log(log_level::warning, "raylib default LoadImage didn't work on: {}", imagepathfromcmd);
                try_unsupported_image_load(img, imagepathfromcmd);
                if (!IsImageValid(img)) {
                    log(log_level::error, "custom format load didn't work either on: {}", imagepathfromcmd);
                }
            }
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

    bool try_unsupported_image_load(Image& loaded, const std::string& dropped_filepath) {
        auto pos = dropped_filepath.find_last_of('.') + 1;
        std::string_view fext(dropped_filepath.data() + pos, dropped_filepath.size() - pos);
        engine::log(engine::log_level::info, "dropped image file extension: {}", fext);
        if (fext == "jfif") { // replace this line with a for loop of all custom extensions
            unsigned char* imgfdata = NULL;
            int a = 0;
            imgfdata = LoadFileData(dropped_filepath.c_str(), &a);
            loaded = LoadImageFromMemory(".jpg", imgfdata, a);
            engine::log(engine::log_level::info, "Loaded a jfif image");
            UnloadFileData(imgfdata);
        }
        if (IsImageValid(loaded)) {
            return true;
        }
        engine::log(engine::log_level::warning, "The jfif image isn't valid");
        return false;
    }

}
