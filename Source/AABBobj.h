#pragma once

#include "CollidableObj.h"

class AABBobj : public CollidableObj
{
public:
	AABBobj() = default;
	~AABBobj() = default;

	const AABoundingBox& getBox() const { return m_boundingBox; }

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;
protected:
	AABoundingBox m_boundingBox;
	bool m_drawBox = false;
};