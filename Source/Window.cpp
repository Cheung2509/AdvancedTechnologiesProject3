#include "Window.h"

#include <WinUser.h>

HINSTANCE Window::m_hInstance = NULL;
std::wstring Window::m_applicationName = L"No Name";

Window::~Window()
{
	UnregisterClass((LPCSTR)m_applicationName.c_str(), m_hInstance);

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

bool Window::init(const wchar_t * className)
{
	if (m_hInstance == NULL)
	{
		m_hInstance = (HINSTANCE)GetModuleHandle(NULL);
	}

	//Initialize the hIcon
	HICON hIcon = NULL;
	WCHAR szExePath[MAX_PATH];
	GetModuleFileName(NULL, (LPSTR)szExePath, 0);

	if (hIcon == NULL)
	{
		hIcon = ExtractIcon(m_hInstance, (LPSTR)szExePath, MAX_PATH);
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
	wndClass.lpfnWndProc = staticWindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = sizeof(Window*);
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = hIcon;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = (LPSTR)m_applicationName.c_str();

	m_hMenu = NULL;

	//Return false if class cannot be registered
	if (!RegisterClass(&wndClass))
	{
		return false;
	}

	m_hWnd = CreateWindow((LPSTR)m_applicationName.c_str(), (LPSTR)m_applicationName.c_str(),
						  WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
						  0, m_hMenu, m_hInstance, this);

	m_hdc = GetDC(m_hWnd);

	return true;
}

LRESULT WINAPI Window::staticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
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

		CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
		Window* const pWnd = reinterpret_cast<Window*>(pcs->lpCreateParams);

		assert(pWnd != nullptr);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pWnd);
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handleThunk));

		return pWnd->handleMsg(hWnd, uMsg, wParam, lParam);
	}


	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT WINAPI Window::handleThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));

	return pWnd->handleMsg(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::handleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
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
		if (!(lParam & 0x40000000) || m_keyboard.autoRepeatIsEnabled())
		{
			m_keyboard.onKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
		m_keyboard.onKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		m_keyboard.onChar(static_cast<unsigned char>(wParam));
		break;

		//Mouse Messages
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);


	}
	break;
	case WM_LBUTTONDOWN:
	{
		m_mouse.onLeftPressed(LOWORD(lParam), HIWORD(lParam));
	}
	break;
	case WM_RBUTTONDOWN:
		m_mouse.onRightPressed(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		m_mouse.onLeftPressed(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONUP:
		m_mouse.onLeftReleased(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			m_mouse.onWheelUp(LOWORD(lParam), HIWORD(lParam));
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			m_mouse.onWhelDown(LOWORD(lParam), HIWORD(lParam));
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
