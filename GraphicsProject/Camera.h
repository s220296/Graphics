#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position);
	~Camera() {}

	void Update(float deltaTime);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);

public:
	float turnSpeed = 0.1f;
	bool invertVerticalCameraMovement = false;

private:
	float theta;
	float phi;
	glm::vec3 position;

	float lastMouseX;
	float lastMouseY;
};

