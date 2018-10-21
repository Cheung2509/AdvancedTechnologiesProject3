#pragma once

#include "VBO.h"
#include "Keyboard.h"
#include "Collidable.h"

class Player : public GameObject3D
{
public:
	Player() = default;
	~Player() = default;

	void init(std::unique_ptr<VBO> model);

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;

	const AABoundingBox& getBoundingBox() const { return m_boundingBox; }

	void handleInput(GameData* keyboard);
private:
	bool collided = false;
	std::unique_ptr<VBO> m_model;
	AABoundingBox m_boundingBox;
};