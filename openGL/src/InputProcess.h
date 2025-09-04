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
};

extern KeyState keyState;

glm::vec3 ProcessInput(glm::vec3 cameraPos, float cameraSpeed);