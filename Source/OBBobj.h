#pragma once

#include "Collidable.h"
#include "GameObject3D.h"

class OBBobj : public GameObject3D
{
public:
	OBBobj() = default;

	static bool checkCollision(const OBBobj& obj1, const OBBobj& obj2);

	void setCollided(const bool& collided) { m_collided = collided; }

	const bool& hasCollided() const { return m_collided; }
	const OBoundingBox& getBox() const { return m_boundingBox; }

	virtual void tick(GameData* gameData) override;
protected:
	bool m_collided = false;
	OBoundingBox m_boundingBox;
};