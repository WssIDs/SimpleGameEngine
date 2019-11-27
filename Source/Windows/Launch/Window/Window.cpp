#include "Window.h"
#include "Resources/resource.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include <iostream>
#include <sstream>

DEFINE_LOG_CATEGORY(WindowLog);

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass()
	: hInst( GetModuleHandle(nullptr))
{
	WNDCLASSEX wc{ 0 };
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof( wc );
	wc.cbWndExtra = NULL;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 64, 64, 0));
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, 0));
	wc.hInstance = getInstance();
	wc.lpszClassName = getName();
	wc.lpszMenuName = nullptr;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = handleMsgSetup;

	RegisterClassEx(&wc);
}

LPCTSTR Window::WindowClass::getName()
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::getInstance()
{
	return wndClass.hInst;
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, getInstance());
}

Window::Window(int width, int height,const std::string& name)
	:width(width),
	height(height)
{
	bConsole = true;
	InitConsole("WGEngine Log");

	RECT wr;

	wr.left = 100;
	wr.right = this->width + wr.left;
	wr.top = 100;
	wr.bottom = this->height + wr.top;

	if(AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false) == 0)
	{
		WGE_LOG(WindowLog, LogVerbosity::Error, "Cannot AdjustWindowRect");
	}

	hwnd = CreateWindow(
		WindowClass::getName(),
		name.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		nullptr,
		nullptr,
		WindowClass::getInstance(),
		this
	);

	ShowWindow(hwnd, SW_SHOWDEFAULT);

	ImGui_ImplWin32_Init(hwnd);
	WGE_LOG(WindowLog, LogVerbosity::Default, "ImGuiWin32 Init");

	pGfx = std::make_unique<Graphics>(hwnd, width, height);

	// register mouse raw input device
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02; // mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
	{
		WGE_LOG(WindowLog, LogVerbosity::Fatal, "Register Raw input Devices failed");
		throw TEXT("Register Raw input Devices failed");
	}

	is_run = true;
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	WGE_LOG(WindowLog, LogVerbosity::Default, "ImGuiWin32 Shutdown");

	DestroyWindow(hwnd);
}

void Window::SetWindowTitle(const std::string& title)
{
	if(SetWindowText(hwnd, title.c_str()) == 0)
	{
		WGE_LOG(WindowLog, LogVerbosity::Error, "Cannot Change Title");
	}
}

void Window::EnableCursor()
{
	cursorEnabled = true;
	ShowCursor();
	EnableImguiMouse();
	FreeCursor();
}

void Window::DisableCursor()
{
	cursorEnabled = false;
	HideCursor();
	DisableImguiMouse();
	ConfineCursor();
}

bool Window::IsCursorEnabled() const
{
	return cursorEnabled;
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;

	while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT)
		{
			return static_cast<int>(msg.wParam);
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return {};
}

Graphics& Window::Gfx()
{
	return *pGfx;
}

bool Window::isRun() const
{
	return is_run;
}

void Window::ConfineCursor()
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	MapWindowPoints(hwnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void Window::FreeCursor()
{
	ClipCursor(nullptr);
}

void Window::ShowCursor()
{
	while (::ShowCursor(true) < 0);
}

void Window::HideCursor()
{
	while (::ShowCursor(false) >= 0);
}

void Window::EnableImguiMouse()
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImguiMouse()
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

void Window::onCreate()
{
	/// When Windows created and show
}

void Window::onUpdate()
{
	// update every frame
}

void Window::onDestroy()
{
	is_run = false;
	// When window destroy
}

void Window::InitConsole(std::string title)
{
	if (bConsole)
	{
		if (AllocConsole())
		{
			FILE* fs;
			SetConsoleTitle(title.c_str());
			freopen_s(&fs, "CON", "w", stdout);
			WGE_LOG(WindowLog, LogVerbosity::Default, "Init Console");

			SetConsoleCtrlHandler(CtrlHandler, TRUE);
		}
		else
		{
			WGE_LOG(WindowLog, LogVerbosity::Error, "Error Console init, Error code = %d", (int)GetLastError());
		}
	}
}

BOOL WINAPI Window::CtrlHandler(DWORD fdwCtrlType)
{
	WGE_LOG(WindowLog, LogVerbosity::Warning, "Handler Console");

	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		return TRUE;

		// CTRL-CLOSE: confirm that the user wants to exit. 
	case CTRL_CLOSE_EVENT:
	{
		WGE_LOG(WindowLog, LogVerbosity::Warning, "Closing Console");
	}
	return TRUE;

	default:
		return FALSE;
	}
}

RECT Window::GetWindowSize() const
{
	RECT rc;
	GetClientRect(hwnd, &rc);

	return rc;
}

LRESULT CALLBACK Window::handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_NCCREATE)
	{
		const auto pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		auto const window = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handleMsgThunk));

		return window->handleMsg(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto const window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return window->handleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(ImGui_ImplWin32_WndProcHandler(hWnd,msg,wParam,lParam))
	{
		return true;
	}

	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, Wnd_OnCreate);
		HANDLE_MSG(hWnd, WM_CLOSE, Wnd_OnClose);
		HANDLE_MSG(hWnd, WM_KILLFOCUS, Wnd_OnKillFocus);
		HANDLE_MSG(hWnd, WM_ACTIVATE, Wnd_OnActivate);
		HANDLE_MSG(hWnd, WM_SIZE, Wnd_OnSize);


		HANDLE_MSG(hWnd, WM_KEYDOWN, Wnd_OnKeyDown);
		HANDLE_MSG(hWnd, WM_SYSKEYDOWN, Wnd_OnKeyDown);
		HANDLE_MSG(hWnd, WM_KEYUP, Wnd_OnKeyUp);
		HANDLE_MSG(hWnd, WM_SYSKEYUP, Wnd_OnKeyUp);
		HANDLE_MSG(hWnd, WM_CHAR, Wnd_OnChar);
		HANDLE_MSG(hWnd, WM_INPUT, Wnd_OnInput);


		HANDLE_MSG(hWnd, WM_MOUSEMOVE, Wnd_OnMouseMove);
		HANDLE_MSG(hWnd, WM_MOUSEWHEEL, Wnd_OnMouseWheel);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, Wnd_OnLeftButtonDown);
		HANDLE_MSG(hWnd, WM_LBUTTONUP, Wnd_OnLeftButtonUp);
		HANDLE_MSG(hWnd, WM_RBUTTONDOWN, Wnd_OnRightButtonDown);
		HANDLE_MSG(hWnd, WM_RBUTTONUP, Wnd_OnRightButtonUp);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL Window::Wnd_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	onCreate();
	return true;
}

void Window::Wnd_OnClose(HWND hwnd)
{
	onDestroy();
	::PostQuitMessage(0);
	FreeConsole();
}

void Window::Wnd_OnKillFocus(HWND hwnd, HWND hwndNewFocus)
{
	keyboardInput.ClearState();
}

void Window::Wnd_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{
	if(!cursorEnabled)
	{
		WGE_LOG(TEXT(WindowLog), LogVerbosity::Default, TEXT("Mouse state = %d"), state);

		if(state == 1)
		{
			WGE_LOG(TEXT(WindowLog), LogVerbosity::Default, TEXT("Cursor confine"));
			ConfineCursor();
			HideCursor();
		}
		else
		{
			WGE_LOG(TEXT(WindowLog), LogVerbosity::Default, TEXT("Cursor free"));
			FreeCursor();
			ShowCursor();
		}
	}
}

void Window::Wnd_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	//S_LOG(TEXT(WindowLog), TEXT("State = %d, cx = %d, cy = %d"), state, cx, cy);
}

void Window::Wnd_OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		if (!(flags & KF_REPEAT) || keyboardInput.AutorepeatIsEnabled()) // filter autorepeat
		{
			keyboardInput.OnKeyPressed(static_cast<unsigned char>(vk));
		}
	}
}

void Window::Wnd_OnKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	if(!ImGui::GetIO().WantCaptureKeyboard)
	{
		keyboardInput.OnKeyReleased(static_cast<unsigned char>(vk));
	}
}

void Window::Wnd_OnChar(HWND hwnd, TCHAR ch, int cRepeat)
{
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		keyboardInput.OnChar(static_cast<unsigned char>(ch));
	}
}


void Window::Wnd_OnInput(HWND hwnd, UINT code, HRAWINPUT hRawInput)
{
	if(!mouseInput.RawEnabled())
	{
		return;
	}

	UINT size = 0;
	if (GetRawInputData(hRawInput, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
	{
		// bail msg processing if error
		return;
	}

	rawBuffer.resize(size);
	if(GetRawInputData(hRawInput, RID_INPUT, rawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
	{
		// bail msg processing if error
		return;
	}

	auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
	if (ri.header.dwType == RIM_TYPEMOUSE &&
		(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
	{
		mouseInput.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
	}
}

/**************** MOUSE MESSAGES *************************/

void Window::Wnd_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
	if(!cursorEnabled)
	{
		if( !mouseInput.IsInWindow())
		{
			SetCapture(hwnd);
			mouseInput.OnMouseEnter();
			HideCursor();
		}
	}
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			mouseInput.OnMouseMove(x, y);
			if (!mouseInput.IsInWindow())
			{
				SetCapture(hwnd);
				mouseInput.OnMouseEnter();
			}
		}
		else
		{
			if ((keyFlags == 1) | (keyFlags == 2)) // left or right button pressed
			{
				mouseInput.OnMouseMove(x, y);
			}
			else
			{
				ReleaseCapture();
				mouseInput.OnMouseLeave();
			}
		}
	}
}

void Window::Wnd_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		mouseInput.OnWheelDelta(xPos, yPos, zDelta);
	}
}

void Window::Wnd_OnLeftButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	SetForegroundWindow(hwnd);
	if(!cursorEnabled)
	{
		WGE_LOG(TEXT(WindowLog), LogVerbosity::Default, TEXT("mouse left click - recapture"));
		ConfineCursor();
		HideCursor();

	}
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		mouseInput.OnLeftPressed(x, y);
	}
}

void Window::Wnd_OnLeftButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		mouseInput.OnLeftReleased(x, y);
	}
}

void Window::Wnd_OnRightButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		mouseInput.OnRightPressed(x, y);
	}
}

void Window::Wnd_OnRightButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		mouseInput.OnRightReleased(x, y);
	}
}

/**************** END MOUSE MESSAGES *************************/