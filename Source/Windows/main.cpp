﻿#include "Launch/Window/ApplicationWindow.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{
	ApplicationWindow application_window(1280, 720, TEXT("WG Engine"));
	application_window.Update(); // Update window every frame
	
	return 0;
}