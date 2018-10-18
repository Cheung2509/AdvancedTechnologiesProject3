#include "Application.h"

#if _DEBUG == 1
#include <iostream>
#endif

#include <fstream>

#include "ErrorHandler.h"
#include "Timer.h"

Application::~Application()
{
}

const bool Application::initialize(const wchar_t* className)
{
	m_window.init(className);

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
	m_renderer->init(m_window.getHWND());

	m_game.init();

	return true;
}

const bool Application::run()
{
	Timer timer;

	if (!IsWindowVisible(m_window.getHWND()))
		ShowWindow(m_window.getHWND(), SW_SHOW);

	// The render loop is controlled here.
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
		m_game.tick(timer.mark(), m_window.getKeyboard(), m_window.getMouse());
		m_game.draw(m_renderer);

		SwapBuffers(m_window.getHDC());
	}

	return true;
}