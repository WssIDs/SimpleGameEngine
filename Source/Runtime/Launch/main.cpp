﻿#include <Runtime/Launch/Window/ApplicationWindow.h>
#include <iostream>


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	ApplicationWindow application_window(1280, 720, "WG Engine",lpCmdLine);
	application_window.Run(); // Update window every frame
	
	return 0;
}