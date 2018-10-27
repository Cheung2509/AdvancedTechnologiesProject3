#pragma once

#include "OBBobj.h"
#include "VBO.h"

class CollidableCube : public OBBobj
{
public:
	CollidableCube() = default;
	~CollidableCube() = default;

	void init(std::unique_ptr<VBO> model);

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;
private:
	std::unique_ptr<VBO> m_model;
};