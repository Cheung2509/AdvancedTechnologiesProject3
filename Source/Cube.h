#pragma once

#include "VBO.h"

class Cube : public VBO
{
public:
	Cube();
	~Cube();

	void init();
	virtual void init(std::string vertexShader, std::string fragmentShader) override;
	virtual void init(std::shared_ptr<Shader> shader) override;

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData)override;
private:
};