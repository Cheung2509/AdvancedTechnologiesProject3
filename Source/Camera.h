#pragma once

#include "GameObject3D.h"

class Camera : public GameObject3D
{
public:
	Camera();
	Camera(const float& fieldOfView, const float& aspectRatio, float nearPlaneDistance,
		   float farPlaneDistance, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), const glm::vec3 target = glm::vec3(0, 0, 0));
	~Camera();

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;

	const glm::mat4& getProjection() const { return m_projectionMatrix; }
	const glm::mat4& getView() const { return m_viewMatrix; }
private:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	float m_fieldOfView;
	float m_aspectRatio;
	float m_nearPlaneDistance;
	float m_farPlaneDistance;

	glm::vec3 m_target;
	glm::vec3 m_up;
};