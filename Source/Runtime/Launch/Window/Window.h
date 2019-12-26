#pragma once
#include <windowsx.h>
#include "WindowHandle.h"
#include <optional> 
#include <memory>
#include <Runtime/Logger/LogDefinitions.h>
#include <Runtime/CoreInput/WindowKeyMessageHandler.h>


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

	Window(int width, int height,const std::string& name,const std::string& commandLine);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void SetWindowTitle(const std::string& title);

	std::string GetWindowName() const;

	void EnableCursor();
	void DisableCursor();

	bool IsCursorEnabled() const;

	// window is run
	bool isRun() const;

	virtual void OnResize();
	virtual void OnPosChange();

	void ToggleBordlessFullScreenMode();

	// loop messages
	std::optional<int> ProcessMessages();

	HWND GetHwnd() const;

	bool IsPaused() const;
	void SetPause(bool newPause);

	bool IsWindowMaximized() const;
	void CloseWindow();

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

	// position window
	struct Position
	{
		int Left;
		int Top;

		Position(int left, int top)
		{
			Left = left;
			Top = top;
		}
	};
	// size window
	struct Size
	{
		int Width;
		int Height;

		Size(int width, int height)
		{
			Width = width;
			Height = height;
		}
	};

	// Create console window
	void InitConsole(std::string& title);
	void InitConsole(std::string& title, Position position);
	void InitConsole(std::string& title, Size size);
	void InitConsole(std::string& title, Position position, Size size);

	void MoveConsole(Position position);
	void ResizeConsole(Size size);

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
	void Wnd_OnEnterSizeMove();
	void Wnd_OnExitSizeMove();
	void Wnd_OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);
	void Wnd_OnWindowPosChanged(HWND hwnd, const LPWINDOWPOS lpwpos);
	BOOL Wnd_Sizing(HWND hwnd, LPRECT lprect);

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
	//KeyboardInput keyboardInput;
	//MouseInput mouseInput;
protected:

	int width = 0;
	int height = 0;
private:

	bool bActive;
	bool bBordlessMaximize = false;
	bool bMinimized;
	bool bMaximized;
	bool bResizing;

	// DX logic
	bool bPaused;

	HWND hwnd;
	bool is_run;
	bool cursorEnabled = true;
	bool bConsole = false;

	std::string windowName;
	//std::unique_ptr<Graphics> pGfx;
	std::vector<unsigned char> rawBuffer;
	std::string commandLine;
};


DECLARE_LOG_CATEGORY_EXTERN(WindowLog)