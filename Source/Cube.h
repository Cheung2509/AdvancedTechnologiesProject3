#pragma once

#include "GameObject3D.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Cube : public GameObject3D
{
public:
	Cube();
	~Cube();

	void init();

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData)override;
private:
	std::unique_ptr<VertexArray> m_va;
	std::unique_ptr<VertexBuffer> m_vb;
	std::unique_ptr<IndexBuffer> m_ib;
	std::unique_ptr<Shader> m_shader;
};