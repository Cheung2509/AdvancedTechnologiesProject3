#include "CollidableObj.h"
#include "AABBobj.h"

CollidableObj::CollidableObj()
{
	//m_isCollidable = true;
}

bool CollidableObj::checkCollision(AABBobj & obj, const glm::vec3 & pos)
{
	return obj.getBox().m_min.x + obj.m_pos.x < pos.x &&
		obj.getBox().m_max.x + obj.m_pos.x > pos.x &&
		obj.getBox().m_min.y + obj.m_pos.y < pos.y &&
		obj.getBox().m_max.y + obj.m_pos.y > pos.y &&
		obj.getBox().m_min.z + obj.m_pos.z < pos.z &&
		obj.getBox().m_max.z + obj.m_pos.z > pos.z;
}

bool CollidableObj::checkCollision(AABBobj & obj1, AABBobj & obj2)
{
	return obj1.getBox().m_max.x + obj1.getPos().x > obj2.getBox().m_min.x + obj2.getPos().x &&
		obj1.getBox().m_min.x + obj1.getPos().x < obj2.getBox().m_max.x + obj2.getPos().x &&
		obj1.getBox().m_max.y + obj1.getPos().y > obj2.getBox().m_min.y + obj2.getPos().y &&
		obj1.getBox().m_min.y + obj1.getPos().y < obj2.getBox().m_max.y + obj2.getPos().y &&
		obj1.getBox().m_max.z + obj1.getPos().z > obj2.getBox().m_min.z + obj2.getPos().z &&
		obj1.getBox().m_min.z + obj1.getPos().z < obj2.getBox().m_max.z + obj2.getPos().z;
}

void CollidableObj::tick(GameData * gameData)
{
	GameObject3D::tick(gameData);
}

void CollidableObj::draw(DrawData * drawData)
{
}
