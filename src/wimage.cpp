#include "raylib.h"
#include <cstring>
#include <engine.hpp>
#include <utility>

namespace engine {
    Texture2D working_image::get_tex() {
        if (!w.is_image_present()) {
            Texture2D empty{};
            engine::log(log_level::warning, "working_image returned an empty texture!");
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
        memset(&img, 0, sizeof(Image));
        img = ImageCopy(image);
        // also unload the temporary image to not have a memory leak
        UnloadImage(image);
        memset(&image, 0, sizeof(Image));
        if (!IsImageValid(img)) {
            engine::log(engine::log_level::error, "Updating working_image failed. ATTEMPTED TO UPDATE WITH:\nPIXEL_FORMAT: {}\nRESOLUTION: ({}x{})", img.format, img.width, img.height);
            w.set_image_false();
        } else { // image was succesfully loaded/replaced
            // this line missing took 2h to debug, 
            w.set_image_true();
        }
        UnloadTexture(img_tex);
        memset(&img_tex, 0, sizeof(Texture2D));
        img_tex = LoadTextureFromImage(img);
        image_too_big_for_gpu = false;
        if (!IsTextureValid(img_tex)) {
            if (IsImageValid(img)) {
                image_too_big_for_gpu = true;
                engine::log(engine::log_level::warning, "Image is too large for GPU, can't use hardware acceleration");
            }
            engine::log(engine::log_level::error, "Couldn't create GPU texture from image when setting new image");
            return;
        }
        engine::log(engine::log_level::info, "Succesfully loaded new image, it should be displaying?");
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
