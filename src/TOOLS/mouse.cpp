#include "mouse.h"
#include "camera.h"

// x posiiton
double Mouse::x = 0;
// y position
double Mouse::y = 0;

// previous x position
double Mouse::lastX = 0;
// previous y position
double Mouse::lastY = 0;

// change in x position from lastX
double Mouse::dx = 0;
// change in y position from lastY
double Mouse::dy = 0;

// change in scroll x
double Mouse::scrollDx = 0;
// change in scroll y
double Mouse::scrollDy = 0;

// if this is the first change in the mouse position
bool Mouse::firstMouse = true;
