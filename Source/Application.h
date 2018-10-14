#pragma once

#ifndef UNICODE
#define UNICODE
#endif

//Pre-Processing directives
#define WIND32_LEAN_AND_MEAN

//Includes
#include <Windows.h>
#include <string>
#include <memory>

#include "Game.h"
#include "Mouse.h"

class Application
{
public:
	Application() = default;
	~Application();

	const bool initialize(const wchar_t* className = L"No Name");

	const bool run();

private:
	static LRESULT WINAPI staticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam,
											 LPARAM lParam);
	static LRESULT WINAPI handleThunk(HWND hWnd, UINT uMsg, WPARAM wParam,
									  LPARAM lParam);
	LRESULT handleMsg(HWND hWnd, UINT uMsg, WPARAM wParam,
					  LPARAM lParam);
private:
	static std::wstring m_applicationName;
	static HINSTANCE m_hInstance;
	HWND m_hWnd;
	HMENU m_hMenu;
	HDC m_hdc;

	std::shared_ptr<Renderer> m_renderer = nullptr;
	Keyboard m_keyboard;
	Mouse m_mouse;
	Game m_game;
}; 