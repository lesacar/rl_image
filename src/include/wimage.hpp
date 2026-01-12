#pragma once
#include <window.hpp>
#include <raylib.h>
#include <optional>

namespace engine {
    class working_image {
        private:
            Image img;
            Texture2D img_tex;
            engine::window& w;
            bool image_too_big_for_gpu = false;
        public:
            // Returns texture if valid, nullopt otherwise
            [[nodiscard("Tried to discard texture without checking validity")]]
            std::optional<Texture2D> get_tex() const;

            // Returns image if valid, nullopt otherwise
            [[nodiscard("Tried to discard image without checking validity")]]
            std::optional<Image> get_image() const;

            // after modifying the image externaly (e.g. rotating), save the updated version, will also try to update the texture
            // returns true if image was successfully set
            bool set_image(Image image);

            working_image() = delete;
            working_image(engine::window& w);
            ~working_image();
    };
}
