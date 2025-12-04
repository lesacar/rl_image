#include "raylib.h"
#include <engine.hpp>

namespace engine {
    Texture2D working_image::get_tex() {
        if (!w.is_image_present()) {
            Texture2D empty{};
            return empty;
        }
        if (!IsTextureValid(img_tex)) {
            log(log_level::error, "Returned invalid texture of working image");
            log(log_level::error, "C++: window.is_image_present() returned true but the texture was invalid, unreachable...");
            std::unreachable();
        }
        return img_tex;
    }

    Image working_image::get_image() {
        if (!w.is_image_present()) {
            Image empty{};
            return empty;
        }
        if (!IsImageValid(img)) {
            log(log_level::error, "Returned invalid image of working image");
            log(log_level::error, "C++: window.is_image_present() returned true but the image was invalid, unreachable...");
            std::unreachable();
        }
        return img;
    }

    // after modifying the image externaly (e.g. rotating), save the updated version, crash on fail
    void working_image::set_image(Image image) {
        UnloadImage(img);
        img = image;
        if (!IsImageValid(img)) {
            engine::log(engine::log_level::error, "Updating working_image failed. ATTEMPTED TO UPDATE WITH:\nPIXEL_FORMAT: {}\nRESOLUTION: ({}x{})", img.format, img.width, img.height);
        }
        UnloadTexture(img_tex);
        img_tex = LoadTextureFromImage(img);
        if (!IsTextureValid(img_tex)) {
            if (IsImageValid(img)) {
                image_too_big_for_gpu = true;
                engine::log(engine::log_level::warning, "Image is too large for GPU, can't use hardware acceleration");
            }
            engine::log(engine::log_level::error, "Couldn't create GPU texture from image when setting new image");
        }
    }

    working_image::working_image(engine::window& w) : w(w) {
        img = image_was_provided(w);
        if (!IsImageValid(img)) {
            engine::log(engine::log_level::info, "engine::working_image class doesn't have a valid image loaded");
            img = {};
            img_tex = {};
            return;
        }
        img_tex = LoadTextureFromImage(img);
        if (!IsTextureValid(img_tex)) {
            engine::log(engine::log_level::error, "Couldn't create GPU texture from image");
            image_too_big_for_gpu = true;
        }
        w.set_image_true();
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
