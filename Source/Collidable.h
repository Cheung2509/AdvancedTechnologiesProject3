#pragma once

#include "glm/glm.hpp"

#include "Helper.h"

struct AABoundingBox
{
	glm::vec3 m_min;
	glm::vec3 m_max;
};

struct BoundingSphere
{
	glm::vec3 m_origin;
	float radius;
};

namespace Collision
{
	static bool hasCollided(const AABoundingBox& obj1, const glm::vec3& pos)
	{
		return obj1.m_min.x < pos.x && obj1.m_max.x > pos.x &&
			obj1.m_min.y < pos.y && obj1.m_max.y > pos.y &&
			obj1.m_min.z < pos.z && obj1.m_max.z > pos.z;
	}

	static bool hasCollided(const BoundingSphere& obj1, const glm::vec3& pos)
	{
		return(glm::distanceSquared(obj1.m_origin, pos) < (obj1.radius * obj1.radius));
	}
}