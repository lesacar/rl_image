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

    std::optional<Image> load_image(window& w)
    {
        if (w.get_cli_args().size() > 1) {
            const char* imagepathfromcmd = w.get_cli_args().at(1).data();
            return load_image_from_path(imagepathfromcmd);
        }

        engine::log(engine::log_level::info, "No image provided from commandline");
        return std::nullopt;
    }

    std::optional<Image> load_image_from_path(std::string_view path)
    {
        // raylib default load
        Image img = LoadImage(path.data());
        if (IsImageValid(img)) {
            return img;
        }

        // try secondary custom load
        log(log_level::warning, "raylib default LoadImage didn't work on: {}", path);
        std::optional<Image> custom_img = load_unsupported_image(std::string(path));

        if (custom_img.has_value()) {
            return custom_img;
        }

        log(log_level::error, "custom format load didn't work either on: {}", path);
        return std::nullopt;
    }

    void DrawTextureMidpoint(Texture2D tex, Vector2 point, float rotation) {
        Rectangle texture = Rectangle{0.0f, 0.0f, static_cast<float>(tex.width), static_cast<float>(tex.height)};
        Rectangle dst = Rectangle{point.x-tex.width/2.0f, point.y-tex.height/2.0f, static_cast<float>(tex.width), static_cast<float>(tex.height)};
        DrawTexturePro(tex, texture, dst, Vector2Zero(), rotation, WHITE);
    }

    std::optional<Image> load_unsupported_image(const std::string& dropped_filepath) {
        auto pos = dropped_filepath.find_last_of('.') + 1;
        std::string_view fext(dropped_filepath.data() + pos, dropped_filepath.size() - pos);
        engine::log(engine::log_level::info, "dropped image file extension: {}", fext);

        if (fext == "jfif") { // replace this line with a for loop of all custom extensions
            unsigned char* imgfdata = NULL;
            int a = 0;
            imgfdata = LoadFileData(dropped_filepath.c_str(), &a);

            if (imgfdata == NULL) {
                engine::log(engine::log_level::warning, "Failed to load file data for: {}", dropped_filepath);
                return std::nullopt;
            }

            Image loaded_img = LoadImageFromMemory(".jpg", imgfdata, a);
            UnloadFileData(imgfdata);

            if (IsImageValid(loaded_img)) {
                engine::log(engine::log_level::info, "Loaded a jfif image");
                return loaded_img;
            }

            engine::log(engine::log_level::warning, "The jfif image isn't valid");
            return std::nullopt;
        }

        // Not a supported custom format
        return std::nullopt;
    }

}
