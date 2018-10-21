#include "AABBobj.h"

bool AABBobj::checkCollision(AABBobj& obj, const glm::vec3& pos)
{
	return obj.getBox().m_min.x + obj.getPos().x < pos.x &&
		obj.getBox().m_max.x + obj.getPos().x > pos.x &&
		obj.getBox().m_min.y + obj.getPos().y < pos.y &&
		obj.getBox().m_max.y + obj.getPos().y > pos.y &&
		obj.getBox().m_min.z + obj.getPos().z < pos.z &&
		obj.getBox().m_max.z + obj.getPos().z > pos.z;
}

bool AABBobj::checkCollision(AABBobj& obj1, AABBobj& obj2)
{
	return obj1.getBox().m_max.x + obj1.getPos().x > obj2.getBox().m_min.x + obj2.getPos().x &&
		obj1.getBox().m_min.x + obj1.getPos().x < obj2.getBox().m_max.x + obj2.getPos().x &&
		obj1.getBox().m_max.y + obj1.getPos().y > obj2.getBox().m_min.y + obj2.getPos().y &&
		obj1.getBox().m_min.y + obj1.getPos().y < obj2.getBox().m_max.y + obj2.getPos().y &&
		obj1.getBox().m_max.z + obj1.getPos().z > obj2.getBox().m_min.z + obj2.getPos().z &&
		obj1.getBox().m_min.z + obj1.getPos().z < obj2.getBox().m_max.z + obj2.getPos().z;

}

void AABBobj::tick(GameData * gameData)
{
	GameObject3D::tick(gameData);
}

void AABBobj::draw(DrawData * drawData)
{
	if (m_drawBox)
	{
		//TODO: DrawBox
	}
}
