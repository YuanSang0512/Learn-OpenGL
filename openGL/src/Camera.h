#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "InputProcess.h"


class Camera
{
public:
	Camera(glm::vec3 pos = {0, 0, 0}, float yaw = -90, float pitch = 0,
		glm::vec3 front = {0, 0, -1}, glm::vec3 up = {0, 1, 0}, glm::vec3 right = {1.0f, 0.0f, 0.0f});

	void CameraUpdate(float deltaTime = 0.3f);

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}

	glm::mat4 GetProjectionMatrix(float aspect, float nearPlane, float farPlane) {
		return glm::perspective(glm::radians(zoom), aspect, nearPlane, farPlane);
	}

	glm::vec3 GetPosition() { return position; }

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp = { 0.0f, 1.0f, 0.0f };

	float yaw;//水平角
	float pitch;//垂直角
	float zoom;//视野
	float movementSpeed;
	float mouseSensitivity;//鼠标灵敏度
	float zoomSensitivity;//滚轮灵敏度
};