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
            bool has_working_image = false;
        public:
            Vector2 mouse_pos_last_frame;
            Vector2 mouse_pos_current_frame;
            std::string name;
            engine::vec2<int> size{};
            Camera2D cam = {};

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
            bool is_image_present();
            void set_image_true();
            void set_image_false();

            // pass argc and argv into the window
            void append_cli_args(size_t argc, const char* argv[]);

            bool should_close();
            void close();
    };
}
