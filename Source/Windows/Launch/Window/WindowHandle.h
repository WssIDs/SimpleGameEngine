#pragma once
#include <windows.h>

/* void Cls_OnInput(HWND hWnd, UINT inputCode, HRAWINPUT hRawInput) */
#define HANDLE_WM_INPUT(hWnd, wParam, lParam, fn) \
  ((fn)((hWnd), GET_RAWINPUT_CODE_WPARAM(wParam), (HRAWINPUT)(lParam)), 0L)

/* void Cls_EnterSizeMove() */
#define HANDLE_WM_ENTERSIZEMOVE(hWnd, wParam, lParam, fn) \
  ((fn)(), 0L)

/* void Cls_ExitSizeMove() */
#define HANDLE_WM_EXITSIZEMOVE(hWnd, wParam, lParam, fn) \
  ((fn)(), 0L)