#include "Window.h"
#include "Resources/resource.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"

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

Window::Window(int width, int height,const TSTRING name)
	:width(width),
	height(height)
{
	RECT wr;

	wr.left = 100;
	wr.right = this->width + wr.left;
	wr.top = 100;
	wr.bottom = this->height + wr.top;

	if(AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false) == 0)
	{
		PRINT_OUTPUT(TEXT("Cannot AdjustWindowRect"));
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
	S_LOG(TEXT("ImGuiWin32"), TEXT("Init"));

	pGfx = std::make_unique<Graphics>(hwnd);

	is_run = true;
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	S_LOG(TEXT("ImGuiWin32"),TEXT("Shutdown"));

	DestroyWindow(hwnd);
}

void Window::SetWindowTitle(const TSTRING& title)
{
	if(SetWindowText(hwnd, title.c_str()) == 0)
	{
		PRINT_OUTPUT(TEXT("Cannot Change Title"));
	}
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


		HANDLE_MSG(hWnd, WM_KEYDOWN, Wnd_OnKeyDown);
		HANDLE_MSG(hWnd, WM_SYSKEYDOWN, Wnd_OnKeyDown);
		HANDLE_MSG(hWnd, WM_KEYUP, Wnd_OnKeyUp);
		HANDLE_MSG(hWnd, WM_SYSKEYUP, Wnd_OnKeyUp);
		HANDLE_MSG(hWnd, WM_CHAR, Wnd_OnChar);


		HANDLE_MSG(hWnd, WM_MOUSEMOVE, Wnd_OnMove);
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
}

void Window::Wnd_OnKillFocus(HWND hwnd, HWND hwndNewFocus)
{
	keyboardInput.ClearState();
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


/**************** MOUSE MESSAGES *************************/

void Window::Wnd_OnMove(HWND hwnd, int x, int y, UINT keyFlags)
{
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