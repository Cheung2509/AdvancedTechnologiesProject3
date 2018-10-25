#pragma once

#include "GameObject3D.h"

class AABBobj;

//Structs for collidables
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

class CollidableObj : public GameObject3D
{
public:
	CollidableObj();
	virtual ~CollidableObj() = default;

	static bool checkCollision(AABBobj& obj, const glm::vec3& pos);
	static bool checkCollision(AABBobj& obj1, AABBobj& obj2);

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;

	const bool& hasCollided() const { return m_hasCollided; }
	void setCollided(const bool& collided) { m_hasCollided = collided; }
protected:
	bool m_hasCollided = false;
};