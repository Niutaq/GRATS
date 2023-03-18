#ifndef MOUSE_H
#define MOUSE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"

class Mouse {
public:

private:
    // x posiiton
    static double x;
    // y position
    static double y;

    // previous x position
    static double lastX;
    // previous y position
    static double lastY;

    // change in x position from lastX
    static double dx;
    // change in y position from lastY
    static double dy;

    // change in scroll x
    static double scrollDx;
    // change in scroll y
    static double scrollDy;

    // if this is the first change in the mouse position
    static bool firstMouse;
};

#endif