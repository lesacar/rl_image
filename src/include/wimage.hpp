#pragma once
#include <window.hpp>
#include <raylib.h>

namespace engine {
    class working_image {
        private:
            Image img;
            Texture2D img_tex;
            engine::window& w;
        public:
            Texture2D get_tex();
            // after modifying the image externaly (e.g. rotating), save the updated version, crash on fail
            void set_image(Image image);
            working_image() = delete;
            working_image(engine::window& w);
            ~working_image();
    };
}
