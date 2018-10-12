#include "Application.h"

#if _DEBUG == 1
#include <iostream>
#endif

#include <fstream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ErrorHandler.h"
#include "Timer.h"

Application::~Application()
{
	UnregisterClass(m_applicationName.c_str(), m_hInstance);

	HMENU hMenu;
	hMenu = GetMenu(m_hWnd);
	if (hMenu != NULL)
	{
		DestroyMenu(hMenu);
	}

	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
	}
}

const bool Application::initialize(const wchar_t* className)
{
	if (m_hInstance == NULL)
	{
		m_hInstance = (HINSTANCE)GetModuleHandle(NULL);
	}

	//Initialize the hIcon
	HICON hIcon = NULL;
	WCHAR szExePath[MAX_PATH];
	GetModuleFileName(NULL, szExePath, 0);

	if (hIcon == NULL)
	{
		hIcon = ExtractIcon(m_hInstance, szExePath, MAX_PATH);
	}

	if (m_hInstance == NULL)
	{
		m_hInstance = GetModuleHandle(NULL);
	}

	//Setting Application name
	m_applicationName = className;

	//Registering Window class
	WNDCLASS wndClass = {};
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = Application::staticWindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = hIcon;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = m_applicationName.c_str();

	m_hMenu = NULL;

	//Return false if class cannot be registered
	if (!RegisterClass(&wndClass))
	{
		return false;
	}

	m_hWnd = CreateWindow(m_applicationName.c_str(), m_applicationName.c_str(),
						  WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
						  0, m_hMenu, m_hInstance, 0);

	m_hdc = GetDC(m_hWnd);

#if _DEBUG == 1
	//Initiate console for debugging
	if (!AllocConsole())
	{
		return false;
	}

	std::ofstream console_out("CONOUT$");
	std::cout.rdbuf(console_out.rdbuf());
#endif
	 
	m_renderer = std::make_shared<Renderer>();
	m_renderer->init(m_hWnd);

	m_game.init();

	return true;
}

const bool Application::run()
{
	Timer timer;

	if (!IsWindowVisible(m_hWnd))
		ShowWindow(m_hWnd, SW_SHOW);

	// The render loop is controlled here.
	bool bGotMsg;
	MSG  msg;
	msg.message = WM_NULL;
	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		while(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			// Translate and dispatch the message
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		m_game.tick(timer.mark());
		m_game.draw(m_renderer);

		SwapBuffers(m_hdc);
	}

	return true;
}

LRESULT Application::staticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		HDC windowHandleToDC = GetDC(hWnd);

		int windowPixelFormat = ChoosePixelFormat(windowHandleToDC, &pfd);
		SetPixelFormat(windowHandleToDC, windowPixelFormat, &pfd);

		break;
	}
	case WM_CLOSE:
	{
		if (hWnd != NULL)
		{
			DestroyWindow(hWnd);
		}
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	//Keyboard Messages
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		break;
	case WM_CHAR:
		break;

	//Mouse Messages
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEWHEEL:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HINSTANCE Application::m_hInstance = NULL;
std::wstring Application::m_applicationName = L"No Name";