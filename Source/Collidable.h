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