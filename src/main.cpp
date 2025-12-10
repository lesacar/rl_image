// TODO: make sure resizing the window also reset's the camera target to window center
// TODO: zoom in towards mouse instead of window center

#include "raylib.h"
#include "timer.hpp"
#include "window.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ranges>
#include <raymath.h>
#include <string>
#include <string_view>

#pragma warning(push, 0)
#pragma warning(disable: 4996 4267 4244 4005 4018 4101 4189 4456 4457 4458 4459 4505 4701 4703)
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#pragma warning(pop)

#include <engine.hpp>

// just an RAII wrapper for some gpu calls you'd want on every frame
class Frame {
private:
    engine::window& window;
public:
    Frame() = delete;
    Frame(engine::window& w);
    ~Frame();
};

Frame::Frame(engine::window& w) : window(w) {
    BeginDrawing();
    ClearBackground(BLACK);
    window.resize_handler();
    window.mouse_pos_current_frame = GetMousePosition();
}

Frame::~Frame() {
    window.mouse_pos_last_frame = window.mouse_pos_current_frame;
    EndDrawing();
}

// process every file drag and dropped to the window, but only extract the first valid filepath, ignore all the rest, use only when you expect only one file
std::string first_dropped_filepath() {
    if (IsFileDropped()) {
        FilePathList list = {};
        std::string droppedFile;
        list = LoadDroppedFiles();
        if (list.count > 0 && list.paths != NULL) {
            engine::log(engine::log_level::info, "dropped files: {}", list.paths[0]);
            droppedFile = list.paths[0];
            if (FileExists(droppedFile.c_str())) {
                UnloadDroppedFiles(list);
                return droppedFile;
            }
        }
        UnloadDroppedFiles(list);
    }
    return std::string{};
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

    engine::log(engine::log_level::info, "window.cam.rotation: {}", window.cam.rotation);


    while (!window.should_close()) {
        Frame frame(window);
        ws.x = static_cast<float>(window.size.x);
        ws.y = static_cast<float>(window.size.y);

        if (IsTextureValid(img.get_tex())) {
            // 0.1f is the strength of the zoom, since the mouse wheel move is a fixed number, e.g. every scroll know could be "2", some mice with more knobs might report it as 0.5 
            float mousey_d = GetMouseWheelMoveV().y * 0.1f;
            window.cam.zoom += window.cam.zoom*mousey_d;
            window.cam.zoom = std::clamp(window.cam.zoom, 0.1f, 10.0f);
            BeginMode2D(window.cam);
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                window.cam.target.x -= GetMouseDelta().x / window.cam.zoom;
                window.cam.target.y -= GetMouseDelta().y / window.cam.zoom;
                // engine::log(engine::log_level::info, "{},{}", window.cam.target.x, window.cam.target.y);
            }
            if (mousey_d != 0) {
                engine::log(engine::log_level::info, "{}", window.cam.zoom);
            }
            if (IsKeyDown(KEY_UP)) {
                rot += 1.0f;
                engine::log(engine::log_level::info, "{}", rot);
            }
            if (IsKeyDown(KEY_DOWN)) {
                rot -= 1.0f;
                engine::log(engine::log_level::info, "{}", rot);
            }


            // if an image has been drag and dropped to the window, set it, if the dropped file wasn't a valid image, the old image will also be removed, black screen until valid image is provided
            std::string dropped_filepath = first_dropped_filepath();
            if (!dropped_filepath.empty()) {
                Image loaded = LoadImage(dropped_filepath.c_str());
                img.set_image(loaded);
            }

            if (IsKeyPressed(KEY_F2)) {
                if (IsWindowState(FLAG_VSYNC_HINT)) {
                    ClearWindowState(FLAG_VSYNC_HINT);
                }
                else {
                    SetWindowState(FLAG_VSYNC_HINT);
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
                        clip_img = LoadImage(ctext.data());
                    }
                }
                if (!ctext.empty()) {
                    engine::log(engine::log_level::info, "Clipboard text: {}", ctext);
                }
                if (IsImageValid(clip_img)) {
                    img.set_image(clip_img);
                }
            }


            engine::DrawTextureMidpoint(img.get_tex(), Vector2Zero(), rot);
            EndMode2D();
        } else { // no image selected
            // draw some kind of file picker for an image or ask the user to dragndrop
        }
        DrawFPS(0, 0);
    }
    return 0;
}
