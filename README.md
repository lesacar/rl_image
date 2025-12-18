## rl_image
Attempt at making an basic image viewer faster then Windows 11's

TODO:
1) if the image resolution is smaller then the GPU's texture limit, load the image as a texture into VRAM,\
otherwise, keep the image in CPU ram, this should improve performance in 99% of cases, and any image\
larger then GPU texsize limit (usually 16384x16384) and smaller then stb_image limit (usually 65535x65535) will be stored in RAM
-- 2) Separate window, working_image and other classes to separate file, engine.cpp is becoming too hard to navigate
3) add basic editing features like lines, hold click to draw, rotate (only 90degree), circles, ...
-- 4) if no image was supplied, add a file picker menu -- drag and drop added as substitute
5) try to load stuff other then OpenGL on a different thread, there isn't much here, a couple milliseconds saved at most

6) quickly zooming in and out crashes the program on a random picture of elden ring
7) when a new image is loaded, if the image size is less then the display resolution, resize the window to the new image

#### TODO: stop using windows for development, MSVC hates compile_commands.json, MSbuild is garbage, the compiler flags always needs edge case exceptions, putting\
CXX_STANDARD 23 from cmake seems to have no effect, continue development on linux with g++ or clang++
## Dependencies
git, cmake4.2 (4.2 is a hard requirement since I use VS 2026)
## Generate build folder
```cmake -DCMAKE_BUILD_TYPE=Release -S . -B build```
## Compile the code
```cmake --build build --config Release```
