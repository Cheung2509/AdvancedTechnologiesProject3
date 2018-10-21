#pragma once

#include "glm/glm.hpp"

namespace glm
{
	static float distanceSquared(glm::vec3 pos1, glm::vec3 pos2)
	{
		vec3 temp = pos1 - pos2;
		float distance = (temp.x * temp.x) + (temp.y * temp.y) + (temp.z * temp.z);
		return distance;
	}
}