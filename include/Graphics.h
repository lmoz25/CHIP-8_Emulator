#pragma once

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <memory>

#include "Logger.h"

#define GRAPHICS_ARRAY_WIDTH 64
#define GRAPHICS_ARRAY_HEIGHT 32
#define MODIFIER 10
class Graphics {
public:
    Graphics();
    ~Graphics();
    void draw(uint8_t *pixels);
    void checkForQuit();
    void updateQuads();
    void reshapeWindow(GLsizei w, GLsizei h);

private:
    void drawPixel(int x, int y);

    int display_width;
    int display_height;
};