#include "Application.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
	//Create new application
	Application* app = new Application();

	//If application is initialized run app
	if (app->initialize(L"Advanced Tech Project"))
	{
		app->run();
	}

	delete app;

	return 0;
}
