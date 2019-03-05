#pragma once

#include <memory>

#include "Keyboard.h"
#include "Mouse.h"

struct GameData
{
	float m_deltaTime;
	float m_runTime = 0.0f;
	Keyboard m_keyboard;
	Mouse m_mouse;
};