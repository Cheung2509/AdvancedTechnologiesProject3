#pragma once

#include <memory>

#include "glm/glm.hpp"

struct GameData;
struct DrawData;
class Renderer;

class GameObject3D
{
public:
	GameObject3D();
	virtual ~GameObject3D();

	virtual void tick(GameData* gameData);
	virtual void draw(DrawData* drawData) = 0;

	//Getters
	const glm::vec3& getPos() const { return m_pos; }
	const glm::vec3& getScale() const { return m_scale; }

	const float& getPitch() const {return m_pitch; }
	const float& getYaw() const { return m_yaw; }
	const float& getRoll() const { return m_roll; }

	//Setters
	void setPos(const glm::vec3& newPos) { m_pos = newPos; }
	void setScale(const glm::vec3& newScale) { m_scale = newScale; }

	void setPitch(const float& newPitch) { m_pitch = newPitch; }
	void setYaw(const float& newYaw) { m_yaw = newYaw; }
	void setRoll(const float& newRoll) { m_roll = newRoll; }
	void setPitchYawRoll(const float& newPitch, const float& newYaw, const float& newRoll)
	{
		m_pitch = newPitch;
		m_yaw = newYaw;
		m_roll = newRoll;
	}
protected:
	glm::vec3 m_pos;
	glm::vec3 m_scale;
	
	float m_pitch = 0;
	float m_yaw = 0;
	float m_roll = 0;

	glm::mat4 m_worldMatrix;
	glm::mat4 m_rotationMatrix;
};