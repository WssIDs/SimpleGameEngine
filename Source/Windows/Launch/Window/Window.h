#pragma once
#include <windows.h>
#include <windowsx.h>
#include "Windows/Input/KeyboardInput.h"
#include "Windows/Input/MouseInput.h"
#include <optional> 
#include "Graphics/DX11/Graphics.h"
#include <memory>

#include "Windows/Logger/LogDefinitions.h"
#include "STypes.h"

/* void Cls_OnInput(HWND hWnd, UINT inputCode, HRAWINPUT hRawInput) */
#define HANDLE_WM_INPUT(hWnd, wParam, lParam, fn) \
  ((fn)((hWnd), GET_RAWINPUT_CODE_WPARAM(wParam), (HRAWINPUT)(lParam)), 0L)


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

	void EnableCursor();
	void DisableCursor();

	bool IsCursorEnabled() const;

	// window is run
	bool isRun() const;
	
	// update window every cycle step
	virtual void onUpdate();

	// loop messages
	std::optional<int> ProcessMessages();

	/// Graphics
	Graphics& Gfx();

protected:

	RECT GetWindowSize() const;

private:
	void ConfineCursor();
	void FreeCursor();
	void ShowCursor();
	void HideCursor();
	void EnableImguiMouse();
	void DisableImguiMouse();

	void onCreate();
	void onDestroy();

	//Console Init
	void InitConsole(std::string title);
	static BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);

	static LRESULT CALLBACK handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	LRESULT handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// window event
	BOOL Wnd_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
	void Wnd_OnClose(HWND hwnd);
	void Wnd_OnKillFocus(HWND hwnd, HWND hwndNewFocus);
	void Wnd_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized);
	void Wnd_OnSize(HWND hwnd, UINT state, int cx, int cy);

	/// keyboard event
	void Wnd_OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
	void Wnd_OnKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
	void Wnd_OnChar(HWND hwnd, TCHAR ch, int cRepeat);
	void Wnd_OnInput(HWND hwnd, UINT code, HRAWINPUT hRawInput);

	// mouse event
	void Wnd_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
	void Wnd_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys);

	void Wnd_OnLeftButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void Wnd_OnLeftButtonUp(HWND hwnd, int x, int y, UINT keyFlags);

	void Wnd_OnRightButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void Wnd_OnRightButtonUp(HWND hwnd, int x, int y, UINT keyFlags);

public:
	KeyboardInput keyboardInput;
	MouseInput mouseInput;
private:

	HWND hwnd;
	bool is_run;
	bool cursorEnabled = true;
	bool bConsole = false;

	int width = 0;
	int height = 0;

	std::unique_ptr<Graphics> pGfx;
	std::vector<BYTE> rawBuffer;
};

