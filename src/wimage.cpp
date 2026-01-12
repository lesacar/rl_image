#include "raylib.h"
#include <cstring>
#include <engine.hpp>
#include <utility>

namespace engine {
    std::optional<Texture2D> working_image::get_tex() const {
        if (!IsTextureValid(img_tex)) {
            engine::log(log_level::warning, "working_image::get_tex() returned nullopt - texture invalid");
            return std::nullopt;
        }
        return img_tex;
    }

    std::optional<Image> working_image::get_image() const {
        if (!IsImageValid(img)) {
            engine::log(log_level::warning, "working_image::get_image() returned nullopt - image invalid");
            return std::nullopt;
        }
        return img;
    }

    // after modifying the image externaly (e.g. rotating), save the updated version
    bool working_image::set_image(Image image) {
        // Unload current image if valid
        if (IsImageValid(img)) {
            UnloadImage(img);
        }
        memset(&img, 0, sizeof(Image));

        // Copy the new image
        img = ImageCopy(image);

        // Unload the source image (it was passed by value, so it's a copy)
        if (IsImageValid(image)) {
            UnloadImage(image);
        }
        memset(&image, 0, sizeof(Image));

        // Image validity check
        if (!IsImageValid(img)) {
            engine::log(engine::log_level::error, "Updating working_image failed. ATTEMPTED TO UPDATE WITH:\nPIXEL_FORMAT: {}\nRESOLUTION: ({}x{})", img.format, img.width, img.height);
            return false;
        }

        // Unload current texture if valid
        if (IsTextureValid(img_tex)) {
            UnloadTexture(img_tex);
        }
        memset(&img_tex, 0, sizeof(Texture2D));

        // Try to create GPU texture
        img_tex = LoadTextureFromImage(img);
        image_too_big_for_gpu = false;

        if (!IsTextureValid(img_tex)) {
            // Texture creation failed (could be size, memory, format, etc.)
            image_too_big_for_gpu = true;
            engine::log(engine::log_level::warning, "GPU texture creation failed (image may be too large or unsupported format)");
            // Still return true because image is valid, just texture failed
        } else {
            engine::log(engine::log_level::info, "Successfully loaded new image with GPU texture");
        }

        // Resize window to fit the new image (if image is valid)
        w.resize_to_fit_image(img);

        return true;
    }

    working_image::working_image(engine::window& w) : w(w) {
        std::optional<Image> loaded_img = load_image(w);
        if (!loaded_img.has_value()) {
            engine::log(engine::log_level::info, "engine::working_image class doesn't have a valid image loaded");
            img = {};
            img_tex = {};
            return;
        }

        img = loaded_img.value();
        img_tex = LoadTextureFromImage(img);
        if (!IsTextureValid(img_tex)) {
            engine::log(engine::log_level::warning, "GPU texture creation failed (image may be too large or unsupported format)");
            image_too_big_for_gpu = true;
        }

        // Resize window to fit the initial image
        w.resize_to_fit_image(img);
    }
    working_image::~working_image() { 
        if (IsImageValid(img)) {
            UnloadImage(img);
        }
        if (IsTextureValid(img_tex)) {
            UnloadTexture(img_tex);
        }
    }
}
