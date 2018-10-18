#pragma once

#include <memory>

#include "Keyboard.h"
#include "Mouse.h"

struct GameData
{
	float m_deltaTime;
	Keyboard m_keyboard;
	Mouse m_mouse;
};