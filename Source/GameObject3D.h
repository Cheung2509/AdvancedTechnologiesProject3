#pragma once

#include <memory>

#include "glm/glm.hpp"

struct GameData;
class Renderer;

class GameObject3D
{
public:
	GameObject3D();
	virtual ~GameObject3D();

	virtual void tick();
	virtual void draw(std::shared_ptr<Renderer> renderer) = 0;
protected:
	glm::vec3 m_pos;
	glm::vec3 m_scale;
	
	float m_pitch = 0;
	float m_yaw = 0;
	float m_roll = 0;

	glm::mat4 m_worldMatrix;
	glm::mat4 m_rotationMatrix;
};