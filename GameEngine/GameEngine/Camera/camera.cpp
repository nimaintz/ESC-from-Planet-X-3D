#include "camera.h"

Camera::Camera(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
	this->rotationOx = 0.0f;
	this->rotationOy = -90.0f;
}

Camera::Camera()
{
	this ->cameraPosition = glm::vec3(0.0f, 25.0f, 100.0f); //start position of our camera x y z
	this ->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this ->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
	this->rotationOx = 0.0f;
	this->rotationOy = -90.0f;
}

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraViewDirection, glm::vec3 cameraUp)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = cameraViewDirection;
	this->cameraUp = cameraUp;
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

Camera::~Camera()
{
}

void Camera::keyboardMoveFront(float cameraSpeed)
{
	cameraPosition += cameraViewDirection * cameraSpeed;
}

void Camera::keyboardMoveBack(float cameraSpeed)
{
	cameraPosition -= cameraViewDirection * cameraSpeed;
}

void Camera::keyboardMoveLeft(float cameraSpeed)
{
	cameraPosition -= cameraRight * cameraSpeed;
}

void Camera::keyboardMoveRight(float cameraSpeed)
{
	cameraPosition += cameraRight * cameraSpeed;
}

void Camera::keyboardMoveUp(float cameraSpeed)
{
	cameraPosition += cameraUp * cameraSpeed;
}

void Camera::keyboardMoveDown(float cameraSpeed)
{
	cameraPosition -= cameraUp * cameraSpeed;
}

void Camera::rotateOx(float angle)
{	
	cameraViewDirection = glm::normalize(glm::vec3((glm::rotate(glm::mat4(1.0f), angle, cameraRight) * glm::vec4(cameraViewDirection, 1))));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraViewDirection));
	cameraRight = glm::cross(cameraViewDirection, cameraUp);

	//// Create a rotation matrix around the cameraRight vector (local X-axis)
	//glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, cameraRight);

	//// Apply the rotation to cameraViewDirection
	//glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(cameraViewDirection, 0.0f);
	//cameraViewDirection = glm::normalize(glm::vec3(rotatedDirection));

	//// Recalculate the up vector to remain perpendicular
	//cameraUp = glm::normalize(glm::cross(cameraRight, cameraViewDirection));
}

void Camera::rotateOy (float angle)
{
	//// Create a rotation matrix around the world up vector (Y-axis)
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, cameraUp);

	//// Apply the rotation to cameraViewDirection
	glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(cameraViewDirection, 0.0f);
	cameraViewDirection = glm::normalize(glm::vec3(rotatedDirection));

	//// Recalculate the right vector to remain perpendicular
	cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(cameraPosition, cameraPosition + cameraViewDirection, cameraUp);
}

glm::vec3 Camera::getCameraPosition()
{
	return cameraPosition;
}

glm::vec3 Camera::getCameraViewDirection()
{
	return cameraViewDirection;
}

glm::vec3 Camera::getCameraUp()
{
	return cameraUp;
}


