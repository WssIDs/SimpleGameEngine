﻿#pragma once
#include <windows.h>
#include <windowsx.h>
#include "Windows/Input/KeyboardInput.h"
#include "Windows/Input/MouseInput.h"
#include <optional> 
#include "Graphics/DX11/Graphics.h"
#include <memory>

#include "Windows/Logger/LogDefinitions.h"
#include "STypes.h"

class Window
{
	// singleton register/cleanup of window class
	class WindowClass
	{
	public:
		static LPCTSTR getName();
		static HINSTANCE getInstance();

	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr LPCTSTR wndClassName = TEXT("WGEngineClass");
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:

	Window(int width, int height,const TSTRING name);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void SetWindowTitle(const TSTRING& title);

	// window is run
	bool isRun() const;
	
	// update window every cycle step
	virtual void onUpdate();

	// loop messages
	std::optional<int> ProcessMessages();

	/// Graphics
	Graphics& Gfx();

protected:
	bool is_run;

	RECT GetWindowSize() const;

public:
	KeyboardInput keyboardInput;
	MouseInput mouseInput;
private:

	HWND hwnd;
	int width = 0;
	int height = 0;

	std::unique_ptr<Graphics> pGfx;

	void onCreate();
	void onDestroy();

	static LRESULT CALLBACK handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	LRESULT handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


	// window event
	BOOL Wnd_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
	void Wnd_OnClose(HWND hwnd);
	void Wnd_OnKillFocus(HWND hwnd, HWND hwndNewFocus);



	/// keyboard event
	void Wnd_OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
	void Wnd_OnKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
	void Wnd_OnChar(HWND hwnd, TCHAR ch, int cRepeat);

	// mouse event
	void Wnd_OnMove(HWND hwnd, int x, int y, UINT keyFlags);
	void Wnd_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys);

	void Wnd_OnLeftButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void Wnd_OnLeftButtonUp(HWND hwnd, int x, int y, UINT keyFlags);

	
	void Wnd_OnRightButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void Wnd_OnRightButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
};

