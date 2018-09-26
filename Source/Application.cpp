#include "Application.h"

#if _DEBUG == 1
#include <iostream>
#include <fstream>
#endif

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

	m_hWnd = CreateWindow(m_applicationName.c_str(), L"Advanced Tech Project",
						  WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
						  0, m_hMenu, m_hInstance, 0);

	m_renderer = std::make_shared<Renderer>();

	if (!m_renderer->init(m_hWnd))
	{
		return false;
	}

#if _DEBUG == 1
	//Initiate console for debugging
	if (!AllocConsole())
	{
		return false;
	}

	std::ofstream console_out("CONOUT$");
	std::cout.rdbuf(console_out.rdbuf());

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
#endif

	return true;
}

const bool Application::run()
{
	if (!IsWindowVisible(m_hWnd))
		ShowWindow(m_hWnd, SW_SHOW);

	// The render loop is controlled here.
	bool bGotMsg;
	MSG  msg;
	msg.message = WM_NULL;
	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		// Process window events.
		// Use PeekMessage() so we can use idle time to render the scene.
		bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

		if (bGotMsg)
		{
			// Translate and dispatch the message
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			HDC dc = GetDC(m_hWnd);
			m_renderer->draw(dc);
		}
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
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HINSTANCE Application::m_hInstance = NULL;
std::wstring Application::m_applicationName = L"No Name";