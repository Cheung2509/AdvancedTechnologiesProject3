#pragma once

#include "VBO.h"
#include "Keyboard.h"
#include "OBBobj.h"

class Player : public OBBobj
{
public:
	Player() = default;
	~Player() = default;

	void init(std::unique_ptr<VBO> model);

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;

	void handleInput(GameData* keyboard);
private:
	std::unique_ptr<VBO> m_model;
};