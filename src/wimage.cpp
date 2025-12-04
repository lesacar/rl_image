#pragma once
#include <engine.hpp>

namespace engine {
    Texture2D working_image::get_tex() { 
        if (!w.is_image_present()) {
            return img_tex;
        }
        if (!IsTextureValid(img_tex)) {
            log(log_level::error, "Returned invalid texture of working image");
            log(log_level::error, "C++: window.is_image_present() returned true but the texture was invalid, unreachable...");
            std::unreachable();
        }
        return img_tex;
    }

    // after modifying the image externaly (e.g. rotating), save the updated version, crash on fail
    void working_image::set_image(Image image) {
        UnloadImage(img);
        img = image;
        if (!IsImageValid(img)) {
            engine::log(engine::log_level::error, "Updating working_image failed. ATTEMPTED TO UPDATE WITH:\nPIXEL_FORMAT: {}\nRESOLUTION: ({}x{})", img.format, img.width, img.height);
            std::exit(-1);
        }
    }

    working_image::working_image(engine::window& w) : w(w) {
        img = image_was_provided(w);
        if (!IsImageValid(img)) {
            engine::log(engine::log_level::info, "engine::working_image class doesn't have a valid image loaded");
            return;
        }
        img_tex = LoadTextureFromImage(img);
        if (!IsTextureValid(img_tex)) {
            engine::log(engine::log_level::error, "Couldn't create GPU texture from image");
        }
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
