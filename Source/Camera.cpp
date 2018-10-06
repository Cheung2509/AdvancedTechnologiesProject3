#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera()
{
}

Camera::Camera(const float & fieldOfView, const float & aspectRatio, float nearPlaneDistance, float farPlaneDistance, const glm::vec3 & up, const glm::vec3 target)
	: m_fieldOfView(fieldOfView), m_aspectRatio(aspectRatio), m_nearPlaneDistance(nearPlaneDistance), m_farPlaneDistance(farPlaneDistance), m_up(up), m_target(target)
{
}

Camera::~Camera()
{
}

void Camera::tick(GameData * gameData)
{
	m_viewMatrix = glm::lookAt(m_pos, m_target, m_up);
	m_projectionMatrix = glm::perspective(m_fieldOfView, m_aspectRatio, m_nearPlaneDistance, m_farPlaneDistance);

	GameObject3D::tick(gameData);
}

void Camera::draw(DrawData* drawData)
{
}
