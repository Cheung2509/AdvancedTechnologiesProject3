#include "Application.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
	//Create new application
	auto app = std::make_unique<Application>();

	//If application is initialized run app
	if (app->initialize(L"Advanced Tech Project"))
	{
		app->run();
	}

	return 0;
}
