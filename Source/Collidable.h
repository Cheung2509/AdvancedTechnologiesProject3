#pragma once

#include "glm/glm.hpp"

#include "Helper.h"

struct AABoundingBox
{
	glm::vec3 m_min; //Min vector of all axes in box
	glm::vec3 m_max; //Max vector of all axes in box
};

struct BoundingSphere
{
	glm::vec3 m_origin;
	float radius; //radius of sphere
};

struct OBoundingBox
{
	glm::vec3 m_origin; //Origin of box (Centre)
	glm::vec3 m_halfLength; //Half length of box
	glm::vec3 m_axes[3]; //Local axes
};