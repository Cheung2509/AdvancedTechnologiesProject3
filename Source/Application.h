#pragma once

#ifndef UNICODE
#define UNICODE
#endif

//Pre-Processing directives
#define WIND32_LEAN_AND_MEAN

//Includes
#include <Windows.h>
#include <string>

class Application
{
public:
	Application() = default;
	~Application();

	const bool initialize(const wchar_t* className = L"No Name");

	const bool run();

	static LRESULT CALLBACK staticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam,
											 LPARAM lParam);
private:
	static std::wstring m_applicationName;
	static HINSTANCE m_hInstance;
	HWND m_hWnd;
	HMENU m_hMenu;

};