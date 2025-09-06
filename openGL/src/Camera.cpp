#include "Camera.h"

Camera::Camera(glm::vec3 pos, float yaw , float pitch,
	glm::vec3 front, glm::vec3 up, glm::vec3 right)
	: movementSpeed(4.0f), mouseSensitivity(0.2f), zoom(70.0f)
{
	position = pos;
	this->yaw = yaw;
	this->pitch = pitch;
	this->front = front;
	this->up = up;
	this->right = right;
}

/// <summary>
/// ����������ĳ����λ�ã����ݵ�ǰ�����ͼ����������������ӽǺ��ƶ���
/// </summary>
void Camera::CameraUpdate(float deltaTime)
{
	if (keyState.table)
	{
		yaw += mouseState.diff_x * mouseSensitivity * deltaTime;
		pitch -= mouseState.diff_y * mouseSensitivity * deltaTime;

		// ���Ƹ����ǣ���ֹ��ת
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;

		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));

		mouseState.diff_x = 0.0;
		mouseState.diff_y = 0.0;
	}
	
	//std::cout << "Camera Front: (" << front.x << ", " << front.y << ", " << front.z << ")\n";
	//std::cout << "Camera Position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";

	if (keyState.w) {
		position += movementSpeed * front;
	}
	if (keyState.a) {
		position -= movementSpeed * right;
	}
	if (keyState.s) {
		position -= movementSpeed * front;
	}
	if (keyState.d) {
		position += movementSpeed * right;
	}
	if (keyState.shift) {
		position -= movementSpeed * worldUp;
	}
	if (keyState.space) {
		position += movementSpeed * worldUp;
	}
}
