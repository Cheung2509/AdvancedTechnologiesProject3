#include "Application.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
	Application* app = new Application();

	if (app->initialize(L"Advanced Tech Project"))
	{
		app->run();
	}

	delete app;

	return 0;
}
