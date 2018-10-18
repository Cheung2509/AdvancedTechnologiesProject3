#pragma once

#ifndef UNICODE
#define UNICODE
#endif

//Pre-Processing directives
#define WIND32_LEAN_AND_MEAN

//Includes
#include "Game.h"
#include "Window.h"

class Application
{
public:
	Application() = default;
	~Application();

	const bool initialize(const wchar_t* className = L"No Name");

	const bool run();

	MSG processMessage();
private:
	std::shared_ptr<Renderer> m_renderer = nullptr;
	Window m_window;
	Game m_game;
}; 