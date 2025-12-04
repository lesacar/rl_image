#include <engine.hpp>
#include <raylib.h>
#include <string_view>

namespace engine {
    bool is_supported_image_extension(std::string_view img_path) {
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
            img = LoadImage(w.get_cli_args().at(1).data());
        }

        if (!IsImageValid(img)) {
            engine::log(engine::log_level::info, "No image provided from commandline");
        }

        return img;
    }
}
