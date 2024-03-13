#include "Camera.h"
#include <glm/ext.hpp>
#include <Input.h>

Camera::Camera() : 
	position(glm::vec3(0)), phi(0), theta(0), lastMouseX(0), lastMouseY(0)
{
}

Camera::Camera(glm::vec3 position) :
	position(position), phi(0), theta(0), lastMouseX(0), lastMouseY(0)
{
}

void Camera::Update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	float thetaR = glm::radians(theta);
	float phiR = glm::radians(phi);

	// Calculate the forwards and right axes and up axis for the camera
	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
	glm::vec3 up(0, 1, 0);

	// use WASDZX keys to move camera around
	// use mouse to rotate camera around
	// if selected (holding right click)
	float mx = input->getMouseX();
	float my = input->getMouseY();

	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		theta += turnSpeed * (mx - lastMouseX);
		float phiDelta = turnSpeed * (my - lastMouseY) * ((invertVerticalCameraMovement) * 2.f - 1);
		phi -= glm::abs(phi + phiDelta) > 70.f ? 0 : phiDelta;

		float speed = 1.f;
		// sprint
		if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT)) speed = 2.3f;
		// only enable movement if clicked into editor
		if (input->isKeyDown(aie::INPUT_KEY_X)) position += up * (deltaTime * speed);
		if (input->isKeyDown(aie::INPUT_KEY_Z)) position -= up * (deltaTime * speed);
		if (input->isKeyDown(aie::INPUT_KEY_W)) position += forward * (deltaTime * speed);
		if (input->isKeyDown(aie::INPUT_KEY_S)) position -= forward * (deltaTime * speed);
		if (input->isKeyDown(aie::INPUT_KEY_D)) position += right * (deltaTime * speed);
		if (input->isKeyDown(aie::INPUT_KEY_A)) position -= right * (deltaTime * speed);
	}

	// stores this frames values for next frame
	lastMouseX = mx;
	lastMouseY = my;
}

glm::mat4 Camera::GetViewMatrix()
{
	float thetaR = glm::radians(theta);
	float phiR = glm::radians(phi);
	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	return glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h)
{
	return glm::perspective(glm::pi<float>() * 0.25f, // 90 degree, PI / 4
		w / h,
		0.1f, 1000.f);
}
