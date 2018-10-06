#pragma once

#include "Camera.h"
#include "Renderer.h"

struct DrawData
{
	std::shared_ptr<Renderer> m_renderer;
	std::shared_ptr<Camera> m_camera;
};