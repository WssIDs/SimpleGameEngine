#include "Window.h"
#include "Resources/resource.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include <iostream>
#include <sstream>
#include "Graphics/DX11/Math/WGMath.h"
#include "Graphics/Helpers/StringHelper.h"
#include "Graphics/Test/TestInputSystem.h"
#include "WindowKeyMessageHandler.h"

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
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
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

Window::Window(int width, int height, const std::string& name, const std::string& commandLine)
	:
	windowName(name),
	width(width),
	height(height),
	commandLine(commandLine)
{
	if (!this->commandLine.empty())
	{
		WGE_LOG(WindowLog, LogVerbosity::Default, "Run with commandline");

		int nArgs;
		const auto pLineW = GetCommandLineW();
		const auto pArgs = CommandLineToArgvW(pLineW, &nArgs);

		if (pArgs == nullptr)
		{
			WGE_LOG(WindowLog, LogVerbosity::Error, "Doesn't parse cammandLine");
		}
		else
		{
			for (int i = 1; i < nArgs; ++i)
			{
				std::string arg = String::ConvertToMultiByte(pArgs[i]);

				if(arg.find("-log") != std::string::npos)
				{
					WGE_LOG(WindowLog, LogVerbosity::Default, "Console mode true");
					bConsole = true;
				}
				else if(arg.find("-res") != std::string::npos)
				{
					std::stringstream resStringStream(arg);
					char delimiter = '=';
					std::string item;
					std::vector<std::string> resSplittedString;

					WGE_LOG(WindowLog, LogVerbosity::Default, "arg = %s", arg.c_str());

					while (std::getline(resStringStream, item, delimiter))
					{
						if (item != "-res")
						{
							resSplittedString.push_back(item);
							WGE_LOG(WindowLog, LogVerbosity::Default, "token = %s", item.c_str());
						}
					}

					if(!resSplittedString.empty())
					{
						std::stringstream resStream(*resSplittedString.begin());
						resSplittedString.clear();
						delimiter = 'x';

						while (std::getline(resStream, item, delimiter))
						{
							if (!item.empty())
							{
								resSplittedString.push_back(item);
							}
						}

						if(resSplittedString.size() >= 2)
						{
							this->width = std::stoi(resSplittedString[0]);
							WGE_LOG(WindowLog, LogVerbosity::Default, "width = %d", this->width);
							this->height = std::stoi(resSplittedString[1]);
							WGE_LOG(WindowLog, LogVerbosity::Default, "height = %d", this->height);
							
							if(2 < resSplittedString.size())
							{
								int bitСolor = std::stoi(resSplittedString[2]);
								WGE_LOG(WindowLog, LogVerbosity::Default, "bitColor = %d", bitСolor);
							}
							if (3 < resSplittedString.size())
							{
								std::string fullscreen = resSplittedString[3];

								if (fullscreen == "f")
								{
									WGE_LOG(WindowLog, LogVerbosity::Default, "bFullScreen = %s", "true");
								}
							}

							WGE_LOG(WindowLog, LogVerbosity::Error, "Resolution will be used in commandline");
						}
						else
						{
							WGE_LOG(WindowLog, LogVerbosity::Error, "resolution not contains");
						}
					}
				}

			}
		}

		//if (nArgs >= 4 && std::wstring(pArgs[1]) == L"-log")
		//{
		//	//const std::wstring pathInWide = pArgs[2];
		//	//const std::wstring pathOutWide = pArgs[3];
		//	//NormalMapTwerker::RotateXAxis180(
		//	//	std::string(pathInWide.begin(), pathInWide.end()),
		//	//	std::string(pathOutWide.begin(), pathOutWide.end())
		//	//);
		//	throw std::runtime_error("Normal map processed successfully. Just kidding about that whole runtime error thing.");
		//}
	}
	else
	{
		WGE_LOG(WindowLog, LogVerbosity::Default, "CommanLine empty");
	}

	std::string logConsoleName = windowName + " Log";

	if (bConsole)
	{
		InitConsole(logConsoleName, Position(0, 0), Size(1100, 350));
	}

	RECT wr = { 0,0, this->width, this->height };

	if(AdjustWindowRectEx(&wr, WS_EX_OVERLAPPEDWINDOW, false, WS_OVERLAPPEDWINDOW) == 0)
	{
		WGE_LOG(WindowLog, LogVerbosity::Error, "Cannot AdjustWindowRect");
	}

	hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		WindowClass::getName(),
		windowName.c_str(),
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

	//RECT rc = GetWindowSize();
	//this->width = rc.right;
	//this->height = rc.bottom;

	ShowWindow(hwnd, SW_SHOWDEFAULT);

	WindowKeyMessageHandler::Get()->SetHwnd(GetHwnd());

	ImGui_ImplWin32_Init(hwnd);
	WGE_LOG(WindowLog, LogVerbosity::Default, "ImGuiWin32 Init");

	Graphics::GetGraphics().InitGraphics(hwnd, this->width, this->height);

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

std::string Window::GetWindowName() const
{
	return windowName;
}

void Window::EnableCursor()
{
	cursorEnabled = true;
	ShowCursor();
	EnableImguiMouse();
	ConfineCursor();
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
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(msg.message == WM_QUIT)
		{
			return static_cast<int>(msg.wParam);
		}
	}
	
	return {};
}

HWND Window::GetHwnd() const
{
	return hwnd;
}

bool Window::IsPaused() const
{
	return bPaused;
}

void Window::SetPause(bool newPause)
{
	bPaused = newPause;
}

bool Window::IsWindowMaximized() const
{
	return bMaximized;
}

void Window::CloseWindow()
{
	if(SendMessage(hwnd,WM_CLOSE,0L,0L) != 0)
	{
		WGE_LOG(WindowLog, LogVerbosity::Error, "Cannot send message to close window, code = %d", (int)GetLastError());
	}
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

void Window::OnResize()
{
}

void Window::OnPosChange()
{
	/// When Windows position changed
}

void Window::ToggleBordlessFullScreenMode()
{
	if (!bBordlessMaximize)
	{
		SetWindowLong(GetHwnd(), GWL_STYLE, WS_POPUP);
		SetWindowLong(GetHwnd(), GWL_EXSTYLE, WS_EX_TOPMOST);
		ShowWindow(GetHwnd(), SW_SHOWMAXIMIZED);

		Graphics::GetGraphics().OnBordlessMaximize();
		ConfineCursor();
		bBordlessMaximize = true;
	}
	else
	{
		SetWindowLong(GetHwnd(), GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowLong(GetHwnd(), GWL_EXSTYLE, WS_EX_OVERLAPPEDWINDOW);
		ShowWindow(GetHwnd(), SW_SHOWDEFAULT);
		ConfineCursor();
		bBordlessMaximize = false;
	}
}

void Window::onDestroy()
{
	is_run = false;
	// When window destroy
}

void Window::InitConsole(std::string& title)
{
	FreeConsole();

	if (AllocConsole())
	{
		FILE *fsIn, *fsOut;
		SetConsoleTitle(title.c_str());
		freopen_s(&fsOut, "CONOUT$", "w", stdout);
		freopen_s(&fsIn, "CONIN$", "r", stdin);
		freopen_s(&fsOut, "CONOUT$", "w", stderr);

		// Note that there is no CONERR$ file
		HANDLE hStdout = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		HANDLE hStdin = CreateFile("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		SetStdHandle(STD_OUTPUT_HANDLE, hStdout);
		SetStdHandle(STD_ERROR_HANDLE, hStdout);
		SetStdHandle(STD_INPUT_HANDLE, hStdin);

		//Clear the error state for each of the C++ standard stream objects. 
		std::wclog.clear();
		std::clog.clear();
		std::wcout.clear();
		std::cout.clear();
		std::wcerr.clear();
		std::cerr.clear();
		std::wcin.clear();
		std::cin.clear();

		WGE_LOG(WindowLog, LogVerbosity::Default, "Init Console");

		SetConsoleCtrlHandler(CtrlHandler, TRUE);
	}
	else
	{
		WGE_LOG(WindowLog, LogVerbosity::Error, "Console cannot init, Error code = %d", (int)GetLastError());
	}
}

void Window::InitConsole(std::string& title, Size size)
{
	InitConsole(title);
	ResizeConsole(size);
}

void Window::InitConsole(std::string& title, Position position)
{
	InitConsole(title);
	MoveConsole(position);
}

void Window::InitConsole(std::string& title, Position position, Size size)
{
	InitConsole(title);
	MoveConsole(position);
	ResizeConsole(size);
}

void Window::MoveConsole(Position position)
{
	HWND consoleWindow = GetConsoleWindow();

	if (consoleWindow)
	{
		SetWindowPos(consoleWindow, nullptr, position.Left, position.Top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
	else
	{
		WGE_LOG(WindowLog, LogVerbosity::Error, "Console not exist, code = %d", (int)GetLastError());
	}
}

void Window::ResizeConsole(Size size)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hConsole)
	{
		auto widthClamped = (SHORT)WGMath::MapRangeClamped(size.Width, 1, 1920, 1, 240);
		auto heightClamped = (SHORT)WGMath::MapRangeClamped(size.Height, 1, 1080, 1, 63);

		SMALL_RECT windowSize = { 0, 0, widthClamped - 1, heightClamped - 1 };

		if (SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize))
		{
			WGE_LOG(WindowLog, LogVerbosity::Success, "SetConsoleWindowInfo");
		}
		else
		{
			WGE_LOG(WindowLog, LogVerbosity::Error, "SetConsoleWindowInfo, code = %d", (int)GetLastError());
		}

		auto heightBuffer = (SHORT)(heightClamped * 500);

		COORD screenBuffer = { widthClamped, heightBuffer };

		if (SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), screenBuffer))
		{
			WGE_LOG(WindowLog, LogVerbosity::Success, "SetConsoleScreenBufferSize");
		}
		else
		{
			WGE_LOG(WindowLog, LogVerbosity::Error, "SetConsoleScreenBufferSize, code = %d", (int)GetLastError());
		}

		CONSOLE_SCREEN_BUFFER_INFO screencsbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screencsbi);

		WGE_LOG(WindowLog, LogVerbosity::Warning, "SetConsoleWindowInfo");
		WGE_LOG(WindowLog, LogVerbosity::Warning, "dwSize.X = %i", screencsbi.dwSize.X);
		WGE_LOG(WindowLog, LogVerbosity::Warning, "dwSize.Y = %i", screencsbi.dwSize.Y);
		WGE_LOG(WindowLog, LogVerbosity::Warning, "srWindow.Bottom = %i", screencsbi.srWindow.Bottom);
		WGE_LOG(WindowLog, LogVerbosity::Warning, "srWindow.Right = %i", screencsbi.srWindow.Right);
	}
	else
	{
		WGE_LOG(WindowLog, LogVerbosity::Error, "Console not exist, code = %d", (int)GetLastError());
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
		HANDLE_MSG(hWnd, WM_ENTERSIZEMOVE, Wnd_OnEnterSizeMove);
		HANDLE_MSG(hWnd, WM_EXITSIZEMOVE, Wnd_OnExitSizeMove);
		HANDLE_MSG(hWnd, WM_GETMINMAXINFO, Wnd_OnGetMinMaxInfo);
		HANDLE_MSG(hWnd, WM_WINDOWPOSCHANGED, Wnd_OnWindowPosChanged);


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
	// display message close
	bPaused = true;
	if (Graphics::GetGraphics().GetTimer())
	{
		Graphics::GetGraphics().GetTimer()->stop();
	}

	onDestroy();
	FreeConsole();
	::PostQuitMessage(0);
}

void Window::Wnd_OnKillFocus(HWND hwnd, HWND hwndNewFocus)
{
	keyboardInput.ClearState();
}

void Window::Wnd_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{
	bActive = (int)state;

	if(bActive == 0)
	{
		FreeCursor();
		ShowCursor();
		SetPause(true);
		if (Graphics::GetGraphics().GetTimer())
		{
			Graphics::GetGraphics().GetTimer()->stop();
		}
		WGE_LOG(WindowLog, LogVerbosity::Default, TEXT("Game Paused"));
	}
	else
	{
		ConfineCursor();
		if (!cursorEnabled)
		{
			WGE_LOG(WindowLog, LogVerbosity::Default, TEXT("Cursor free"));
			HideCursor();
		}
		SetPause(false);
		if (Graphics::GetGraphics().GetTimer())
		{
			Graphics::GetGraphics().GetTimer()->start();
		}
		WGE_LOG(WindowLog, LogVerbosity::Default, TEXT("Game Resumed"));
	}
}

void Window::Wnd_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	if(state == 1)
	{
		bMinimized = true;
		bMinimized = false;
		bPaused = true;
	}
	else if (state == 2)
	{
		bMinimized = false;
		bMaximized = true;
		OnResize();
		bPaused = false;
	}
	else if (state == 0)
	{
		if (bMinimized)
		{
			bMinimized = false;
			OnResize();
			bPaused = false;
		}
		else if (bMaximized)
		{
			bMaximized = false;
			OnResize();
			bPaused = false;
		}
		else if (bResizing)
		{

		}
		else
			OnResize();
	}
}

void Window::Wnd_OnEnterSizeMove()
{
	WGE_LOG(WindowLog, LogVerbosity::Default, "EnterSizeMove");
	bResizing = true;
	bPaused = true;

	if (Graphics::GetGraphics().GetTimer())
	{
		Graphics::GetGraphics().GetTimer()->stop();
	}
}

void Window::Wnd_OnExitSizeMove()
{
	WGE_LOG(WindowLog, LogVerbosity::Default, "ExitSizeMove");
	bResizing = false;
	OnResize();
	bPaused = false;
	if (Graphics::GetGraphics().GetTimer())
	{
		Graphics::GetGraphics().GetTimer()->start();
	}
}

void Window::Wnd_OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
{
	lpMinMaxInfo->ptMinTrackSize.x = 200;
	lpMinMaxInfo->ptMinTrackSize.y = 200;
}

void Window::Wnd_OnWindowPosChanged(HWND hwnd, const LPWINDOWPOS lpwpos)
{
	OnPosChange();
}

BOOL Window::Wnd_Sizing(HWND hwnd, LPRECT lprect)
{
	return false;
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
	//if(!mouseInput.RawEnabled())
	//{
	//	return;
	//}

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
		WindowKeyMessageHandler::Get()->ProcessRaw(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
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
	//if (!ImGui::GetIO().WantCaptureKeyboard)
	//{
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
	//}
}

void Window::Wnd_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{
	if (!ImGui::IsAnyItemActive())
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
		//TestInputSystem::Get().Execute("LeftButtonPress");
	}
}

void Window::Wnd_OnLeftButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		mouseInput.OnLeftReleased(x, y);
		//TestInputSystem::Get().Execute("LeftButtonRelease");
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