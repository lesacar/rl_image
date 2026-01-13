// TODO: extend load_unsupported_image to other formats that raylib doesn't support, but stbi does (currently just jfif), eventualy replace raylib LoadImage
// with LoadImageFromMemory for every single format
// TODO: remove is_image_present, set_image_true, and set_image_false abstractions, much easier to rely on raylib's
// IsImageValid function

#include "common_types.h"
#include "raylib.h"
#include "timer.hpp"
#include "window.hpp"
#include "frame.hpp"
#include <algorithm>
#include <cstdlib>
#include <string>
#include <string_view>

#ifdef _WIN32
#pragma warning(push, 0)
#pragma warning(disable: 4996 4267 4244 4005 4018 4101 4189 4456 4457 4458 4459 4505 4701 4703)
#endif // _WIN32

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif // __GNUC__

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <raymath.h>
#ifdef _WIN32
#pragma warning(pop)
#endif // _WIN32

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__

#include <engine.hpp>


// process every file drag and dropped to the window, but only extract the first valid filepath, ignore all the rest, use only when you expect only one file
std::optional<std::string> first_dropped_filepath() {
    if (!IsFileDropped()) {
        return std::nullopt;
    }

    FilePathList list = LoadDroppedFiles();
    if (list.count == 0 || list.paths == NULL) {
        UnloadDroppedFiles(list);
        return std::nullopt;
    }

    std::string droppedFile = list.paths[0];
    engine::log(engine::log_level::info, "dropped file: {}", droppedFile);

    UnloadDroppedFiles(list);
    return droppedFile;
}

int main(int argc, const char* argv[]) {
    static_assert(sizeof(engine::vec2<float>) == sizeof(Vector2), "raylib Vector2 is not the same as engine::vec2<float>");
    engine::timer timmy{false};
    SetTraceLogLevel(LOG_WARNING);

    timmy.elapsed_ms();
    engine::window window{"Image Viewer"};
    timmy.elapsed_ms();
    window.append_cli_args(argc, argv);
    // SetTargetFPS(0);

    // engine::working_image img;
    engine::working_image img(window);

    Vector2 halfer = Vector2{2.0f,2.0f};
    Vector2 ws = Vector2{static_cast<float>(window.size.x), static_cast<float>(window.size.y)};

    window.cam.zoom = 1.0f;
    window.cam.target = Vector2Zero();
    window.cam.offset = Vector2Divide(ws, halfer);
    float rot = 0.0f;

    // engine::log(engine::log_level::info, "window.cam.rotation: {}", window.cam.rotation);

    while (!window.should_close()) {
        Frame frame(window);
        ws.x = static_cast<float>(window.size.x);
        ws.y = static_cast<float>(window.size.y);

        std::optional<Texture2D> texture_opt = img.get_tex();
        if (texture_opt.has_value()) {
            Texture2D texture = texture_opt.value();
            // 0.1f is the strength of the zoom, since the mouse wheel move is a fixed number, e.g. every scroll know could be "2", some mice with more knobs might report it as 0.5 
            float mousey_d = GetMouseWheelMoveV().y * 0.1f;
            if (mousey_d != 0.0f) { // the user scrolled the mouse wheel
                // get the difference in camera position before and after zooming in, then restore the old camera target
                // this way we zoom in AND keep the same pixel the camera was pointing to

                // window.cam.target = GetScreenToWorld2D(GetMousePosition(), window.cam);
                Vector2 before_zoom = GetScreenToWorld2D(GetMousePosition(), window.cam);

                window.cam.zoom += window.cam.zoom*mousey_d;
                window.cam.zoom = std::clamp(window.cam.zoom, 0.1f, 10.0f);

                Vector2 after_zoom = GetScreenToWorld2D(GetMousePosition(), window.cam);

                window.cam.target += before_zoom-after_zoom;
            }
            // engine::log(engine::log_level::info, "window.cam.target = {},{}\n", window.cam.target.x, window.cam.target.y);

            BeginMode2D(window.cam);
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                window.cam.target.x -= GetMouseDelta().x / window.cam.zoom;
                window.cam.target.y -= GetMouseDelta().y / window.cam.zoom;
                // engine::log(engine::log_level::info, "{},{}", window.cam.target.x, window.cam.target.y);
            }
            /*
            if (mousey_d != 0) {
                engine::log(engine::log_level::info, "{}", window.cam.zoom);
            } */
            if (IsKeyDown(KEY_UP)) {
                rot += 1.0f;
                engine::log(engine::log_level::info, "{}", rot);
            }
            if (IsKeyDown(KEY_DOWN)) {
                rot -= 1.0f;
                engine::log(engine::log_level::info, "{}", rot);
            }
            if (IsKeyDown(KEY_SPACE)) {
                window.cam.target = Vector2Zero();
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                Vector2 mpos = GetMousePosition();
                Vector2 wmpos = GetScreenToWorld2D(mpos, window.cam);
                engine::log(engine::log_level::info, "Mouse world position: {},{}", wmpos.x,wmpos.y);
            }


            if (IsKeyPressed(KEY_F2)) {
                if (IsWindowState(FLAG_VSYNC_HINT)) {
                    ClearWindowState(FLAG_VSYNC_HINT);
                }
                else {
                    SetWindowState(FLAG_VSYNC_HINT);
                }
            }

            if (IsKeyPressed(KEY_F11)) {
                std::optional<Image> current_image = img.get_image();
                if (current_image.has_value()) {
                    window.toggle_borderless(&current_image.value());
                } else {
                    window.toggle_borderless(nullptr);
                }
            }

            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V)) {
                // either an image is directly in the clipboard, so clip_img will be valid
                // or a filepath is in the clipboard, so clip_img will first be invalid, then made valid later, no overwriting should happen in the FileExists check
                Image clip_img = GetClipboardImage();
                const char* p_clipboard_text = GetClipboardText();
                std::string_view ctext;

                if (p_clipboard_text) {
                    ctext = p_clipboard_text;
                    if (FileExists(ctext.data())) {
                        // Try to load from filepath in clipboard
                        std::optional<Image> file_img = engine::load_image_from_path(ctext);
                        if (file_img.has_value()) {
                            clip_img = file_img.value();
                        }
                    }
                }

                if (!ctext.empty()) {
                    engine::log(engine::log_level::info, "Clipboard text: {}", ctext);
                }

                if (IsImageValid(clip_img)) {
                    // set_image() will resize window internally if successful
                    img.set_image(clip_img);
                }
            }


            engine::DrawTextureMidpoint(texture, Vector2Zero(), rot);
            EndMode2D();
        } else { // no image selected
            std::string_view placeholder_text = "Drag and Drop any image to display it";
            engine::vec2<int> ph_pos = {};
            Vector2 ph_size = MeasureTextEx(GetFontDefault(), placeholder_text.data(), 24, 1.0f);
            // Center text: use current RENDER area size (accounts for HiDPI)
            int render_width = GetRenderWidth();
            int render_height = GetRenderHeight();
            ph_pos.x = render_width/2 - static_cast<int>(ph_size.x)/2;
            ph_pos.y = render_height/2 - static_cast<int>(ph_size.y)/2;

            // Debug: log positioning info (simplified to avoid MSVC format issues)
            engine::log(engine::log_level::info,
                       "Text position: render {}x{}, text pos {}x{}",
                       render_width, render_height, ph_pos.x, ph_pos.y);

            // Draw text centered using DrawTextPro for precise positioning
            Vector2 origin = {0, 0}; // Top-left origin
            DrawTextPro(GetFontDefault(), placeholder_text.data(),
                       Vector2{static_cast<float>(ph_pos.x), static_cast<float>(ph_pos.y)},
                       origin, 0.0f, 24, 1.0f, GREEN);
            // draw some kind of file picker for an image or ask the user to dragndrop
        }

        // if an image has been drag and dropped to the window, set it, if the dropped file wasn't a valid image, the old image will also be removed, black screen until valid image is provided
        std::optional<std::string> dropped_filepath_opt = first_dropped_filepath();
        if (dropped_filepath_opt.has_value()) {
            std::string dropped_filepath = dropped_filepath_opt.value();
            // Try to load image using new std::optional API
            std::optional<Image> loaded_opt = engine::load_image_from_path(dropped_filepath);

            if (!loaded_opt.has_value()) {
                engine::log(engine::log_level::warning, "Image {} wasn't loaded by raylib or with a custom format!", dropped_filepath);
            } else {
                Image loaded_img = loaded_opt.value();
                // set_image() will resize window internally if successful
                img.set_image(loaded_img);
            }
        }
        
        if (IsKeyPressed(KEY_F3)) {
            window.toggle_show_fps();
        }
    }
    return 0;
}
