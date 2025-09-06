#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>


struct KeyState {
    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;
	bool shift = false;
	bool space = false;
	bool table = false;
};

struct MouseState {
    double x = 0.0;
    double y = 0.0;
	double diff_x = 0.0;
	double diff_y = 0.0;
    bool mounseControl = false;
};

extern KeyState keyState;
extern MouseState mouseState;