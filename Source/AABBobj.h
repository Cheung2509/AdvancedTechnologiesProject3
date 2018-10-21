#pragma once

#include "GameObject3D.h"
#include "Collidable.h"

class AABBobj : public GameObject3D
{
public:
	AABBobj() = default;
	~AABBobj() = default;

	void setCollided(const bool& collided) { m_collided = collided; }

	const AABoundingBox& getBox() const { return m_boundingBox; }
	const bool& hasCollided() const { return m_collided; }

	static bool checkCollision(AABBobj& obj, const glm::vec3& pos);
	static bool checkCollision(AABBobj& obj1, AABBobj& obj2);

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;
protected:
	bool m_collided = false;
	bool m_drawBox = false;
	AABoundingBox m_boundingBox;
};