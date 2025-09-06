#include "InputProcess.h"

KeyState keyState;



glm::vec3 ProcessInput(glm::vec3 cameraPos, float cameraSpeed)
{
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    if (keyState.w) {
        cameraPos += cameraSpeed * cameraFront;
		std::cout << "Camera Position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")\n";
    }
    if (keyState.a) {
		cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
        std::cout << "Camera Position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")\n";
    }
    if (keyState.s) {
        cameraPos -= cameraSpeed * cameraFront;
        std::cout << "Camera Position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")\n";
    }
    if (keyState.d) {
        cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
        std::cout << "Camera Position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")\n";
    }
	if (keyState.shift) {
		cameraPos -= cameraSpeed * cameraUp;
        std::cout << "Camera Position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")\n";
    }
    if(keyState.space) {
        cameraPos += cameraSpeed * cameraUp;
        std::cout << "Camera Position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")\n";
    }
    return cameraPos;
}