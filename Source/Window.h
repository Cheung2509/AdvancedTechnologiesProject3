#pragma once

#include <Windows.h>

#include "Mouse.h"
#include "Keyboard.h"

class Window
{
public:
	Window() = default;
	~Window();

	bool init(const wchar_t* className);
	
	const Keyboard& getKeyboard() const { return m_keyboard; }
	const Mouse& getMouse() const { return m_mouse; }
	HDC getHDC() const { return m_hdc; }
	HWND& getHWND() { return m_hWnd; }
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

	Mouse m_mouse;
	Keyboard m_keyboard;
};