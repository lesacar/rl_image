#pragma once
#include "raylib.h"
#include <string>
#include <string_view>
#include <vector>
#include <common_types.h>

namespace engine {
    class window {
        private:
            std::vector<std::string_view> cli_args;
        public:
            Vector2 mouse_pos_last_frame;
            Vector2 mouse_pos_current_frame;
            engine::vec2<int> size{};
            bool show_fps = false; // actual FPS draw handled in Frame::~Frame() to make sure the FPS is always on top
            Camera2D cam = {};
            std::string name;

            window() = delete;
            window(engine::vec2<int> size, std::string_view name);
            window(std::string_view name);
            ~window();

            void pre_initialization();
            std::vector<std::string_view>& get_cli_args();
            void post_initialization();
            void set_best_fit_resolution();
            void set_resize_to(engine::vec2<int> newSize);
            void center_to_monitor();
            void resize_handler();
            bool toggle_show_fps();
            bool toggle_show_fps(bool new_show_fps);

            // Resize window to fit image dimensions
            bool resize_to_fit_image(const Image& img);
            

            // pass argc and argv into the window
            void append_cli_args(size_t argc, const char* argv[]);

            bool should_close();
            void close();
    };
}
