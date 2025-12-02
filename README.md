## rl_image
Attempt at making an basic image viewer faster then Windows 11's

TODO:
1) if the image resolution is smaller then the GPU's texture limit, load the image as a texture into VRAM,\
otherwise, keep the image in CPU ram, this should improve performance in 99% of cases, and any image\
larger then GPU texsize limit (usually 16384x16384) and smaller then stb_image limit (usually 65535x65535) will be stored in RAM
2) add basic editing features like lines, hold click to draw, rotate (only 90degree), circles, ...
3) if no image was supplied, add a file picker menu
4) try to load stuff other then OpenGL on a different thread, there isn't much here, a couple milliseconds saved at most

## Dependencies
git, cmake4.2 (4.2 is a hard requirement since I use VS 2026)
## Generate build folder
```cmake -DCMAKE_BUILD_TYPE=Release -S . -B build```
## Compile the code
```cmake --build build --config Release```
