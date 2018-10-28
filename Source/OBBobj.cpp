#include "OBBobj.h"

#include <algorithm>

#include "glm/gtc/quaternion.hpp"

bool OBBobj::checkCollision(const OBBobj& obj1, const OBBobj& obj2)
{
	float ra;
	float rb;

	glm::mat3 r;
	glm::mat3 AbsR;

	//Get rotation matrix of the box
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			r[i][j] = glm::dot(obj1.getBox().m_axes[i], obj2.getBox().m_axes[j]);
		}
	}

	glm::vec3 t = obj2.getPos() - obj1.getPos();

	t = glm::vec3(glm::dot(t, obj1.getBox().m_axes[0]), 
				  glm::dot(t, obj1.getBox().m_axes[1]), 
				  glm::dot(t, obj1.getBox().m_axes[2]));

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			AbsR[i][j] = std::abs(r[i][j]) + std::numeric_limits<float>::epsilon();
		}
	}

	for (int i = 0; i < 3; i++)
	{
		ra = obj1.getBox().m_halfLength[i];
		rb = obj2.getBox().m_halfLength[0] * AbsR[i][0] +
			obj2.getBox().m_halfLength[1] * AbsR[i][1] +
			obj2.getBox().m_halfLength[2] * AbsR[i][2];

		if (std::abs(t[i]) > ra + rb)
		{
			return false;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		ra = obj1.getBox().m_halfLength[0] * AbsR[0][i] +
			obj1.getBox().m_halfLength[1] * AbsR[1][i] +
			obj1.getBox().m_halfLength[2] * AbsR[2][i];
		rb = obj2.getBox().m_halfLength[i];

		if (std::abs(t[0] * r[0][i] + t[1] * r[1][i] + t[2] * r[2][i])
	> ra + rb)
		{
			return false;
		}
	}

	//L = A0 x B0
	ra = obj1.getBox().m_halfLength[1] * AbsR[2][0] +
		obj1.getBox().m_halfLength[2] * AbsR[1][0];
	rb = obj2.getBox().m_halfLength[1] * AbsR[0][2] +
		obj2.getBox().m_halfLength[2] * AbsR[0][1];
	if (std::abs(t[2] * r[1][0] - t[1] * r[2][0]) > ra + rb)
	{
		return false;
	}

	//L = A0 x B1 
	ra = obj1.getBox().m_halfLength[1] * AbsR[2][1] +
		obj1.getBox().m_halfLength[2] * AbsR[1][1];
	rb = obj2.getBox().m_halfLength[0] * AbsR[0][2] +
		obj2.getBox().m_halfLength[2] * AbsR[0][0];
	if (std::abs(t[2] * r[1][1] - t[1] * r[2][1]) > ra + rb)
	{
		return false;
	}

	//L = A0 x B2
	ra = obj1.getBox().m_halfLength[1] * AbsR[2][2] +
		obj1.getBox().m_halfLength[2] * AbsR[1][2];
	rb = obj2.getBox().m_halfLength[0] * AbsR[0][1] +
		obj2.getBox().m_halfLength[1] * AbsR[0][0];
	if (std::abs(t[2] * r[1][2] - t[1] * r[2][2]) > ra + rb)
	{
		return false;
	}

	//L = A1 x B0
	ra = obj1.getBox().m_halfLength[0] * AbsR[2][0] +
		obj1.getBox().m_halfLength[2] * AbsR[0][0];
	rb = obj2.getBox().m_halfLength[1] * AbsR[1][2] +
		obj2.getBox().m_halfLength[2] * AbsR[1][1];
	if (std::abs(t[0] * r[2][0] - t[2] * r[0][0]) > ra + rb)
	{
		return false;
	}

	//L A1 x B1
	ra = obj1.getBox().m_halfLength[0] * AbsR[2][1] +
		obj1.getBox().m_halfLength[2] * AbsR[0][1];
	rb = obj2.getBox().m_halfLength[0] * AbsR[1][2] +
		obj2.getBox().m_halfLength[2] * AbsR[1][0];
	if (std::abs(t[0] * r[2][1] - t[2] * r[0][1]) > ra + rb)
	{
		return false;
	}

	//L = A1 x B2
	ra = obj1.getBox().m_halfLength[0] * AbsR[2][2] +
		obj1.getBox().m_halfLength[2] * AbsR[0][2];
	rb = obj2.getBox().m_halfLength[0] * AbsR[1][1] +
		obj2.getBox().m_halfLength[1] * AbsR[1][0];
	if (std::abs(t[0] * r[2][2] - t[2] * r[0][2]) > ra + rb)
	{
		return false;
	}

	//L = A2 x B0
	ra = obj1.getBox().m_halfLength[0] * AbsR[1][0] +
		obj1.getBox().m_halfLength[1] * AbsR[0][0];
	rb = obj2.getBox().m_halfLength[1] * AbsR[2][2] +
		obj2.getBox().m_halfLength[2] * AbsR[2][1];
	if (std::abs(t[1] * r[0][0] - t[0] * r[1][0]) > ra + rb)
	{
		return false;
	}

	//L = A2 x B1
	ra = obj1.getBox().m_halfLength[0] * AbsR[1][1] +
		obj1.getBox().m_halfLength[1] * AbsR[0][1];
	rb = obj2.getBox().m_halfLength[0] * AbsR[2][2] +
		obj2.getBox().m_halfLength[2] * AbsR[2][0];
	if (std::abs(t[1] * r[0][1] - t[0] * r[1][1]) > ra + rb)
	{
		return false;
	}

	//: = A2 x B2
	ra = obj1.getBox().m_halfLength[0] * AbsR[1][2] +
		obj1.getBox().m_halfLength[1] * AbsR[0][2];
	rb = obj2.getBox().m_halfLength[0] * AbsR[2][1] +
		obj2.getBox().m_halfLength[1] * AbsR[2][0];
	if (std::abs(t[1] * r[0][2] - t[0] * r[1][2]) > ra + rb)
	{
		return false;
	}

	return true;
}

void OBBobj::tick(GameData * gameData)
{
	GameObject3D::tick(gameData);

	glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(0.0f, 0.0f, 1.0f);

	m_boundingBox.m_axes[0] = glm::normalize(forward * m_rotation);
	m_boundingBox.m_axes[1] = glm::normalize(up * m_rotation);
	m_boundingBox.m_axes[2] = glm::normalize(right * m_rotation);
}
