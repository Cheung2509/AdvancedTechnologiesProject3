#include "GameObject3D.h"

#include "glm/gtc/matrix_transform.hpp"

#include "GameData.h"


GameObject3D::GameObject3D()
	:m_worldMatrix(1.0f), m_scale(1.0f,1.0f,1.0f), m_rotationMatrix(1.0f),
	m_rotation(1.0f,0.0f,0.0f,0.0f)
{
}

GameObject3D::~GameObject3D()
{
}

void GameObject3D::tick(GameData * gameData)
{	
	m_rotationMatrix = glm::toMat4(m_rotation);

	glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), m_pos);
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), m_scale);

	m_worldMatrix = translationMat * m_rotationMatrix * scaleMat;
}
