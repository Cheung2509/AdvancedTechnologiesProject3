#pragma once

#include "VBO.h"

class Cube : public VBO
{
public:
	Cube();
	Cube(std::shared_ptr<Shader> shader);
	~Cube();

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData)override;
private:
};