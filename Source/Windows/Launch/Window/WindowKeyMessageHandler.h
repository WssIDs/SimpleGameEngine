#pragma once
#include <stack>
#include "Graphics\Test\TestInputSystem.h"
#include "Graphics\Engine\Core.h"
#include <set>
#include <vector>
#include "Graphics\Helpers\StringHelper.h"
#include <map>
#include <memory>
#include <Graphics\Test\InputTypes.h>

struct FKeyEvent
{
	FKeyEvent()
	{
	}

	FKeyEvent(FKey Key, bool bShift = false, bool bAlt = false, bool bCtrl = false, bool bCmd = false)
		:
		Key(Key),
		bShift(bShift),
		bAlt(bAlt),
		bCtrl(bCtrl),
		bCmd(bCmd)
	{}

	FKey Key;

	void ProcessModifierKeys()
	{
		bShift = ProcessModifierKeyDown(LeftShiftKey) | ProcessModifierKeyDown(RightShiftKey);
		bAlt = ProcessModifierKeyDown(LeftAltKey) | ProcessModifierKeyDown(RightAltKey);
		bCtrl = ProcessModifierKeyDown(LeftCtrlKey) | ProcessModifierKeyDown(RightCtrlKey);
		bCmd = ProcessModifierKeyDown(LeftCmdKey) | ProcessModifierKeyDown(RightCmdKey);
	}

	bool ProcessModifierKeyDown(FKey inKey);

	bool IsShiftDown() const
	{
		return bShift;
	}

	bool IsAltDown() const
	{
		return bAlt;
	}

	bool IsControlDown() const
	{
		return bCtrl;
	}

	bool IsCommandDown() const
	{
		return bCmd;
	}

	unsigned short bShift = 0;
	unsigned short bAlt = 0;
	unsigned short bCtrl = 0;
	unsigned short bCmd = 0;

	FKey LeftShiftKey = EKeys::LeftShift;
	FKey RightShiftKey = EKeys::RightShift;

	FKey LeftAltKey = EKeys::LeftAlt;
	FKey RightAltKey = EKeys::RightAlt;

	FKey LeftCtrlKey = EKeys::LeftControl;
	FKey RightCtrlKey = EKeys::RightControl;

	FKey LeftCmdKey = EKeys::LeftCommand;
	FKey RightCmdKey = EKeys::RightCommand;
};

class WindowKeyMessageHandler
{
public:
	WindowKeyMessageHandler(const WindowKeyMessageHandler&) = delete;
	WindowKeyMessageHandler& operator=(const WindowKeyMessageHandler&) = delete;
	~WindowKeyMessageHandler();

	void SetHwnd(HWND hwnd);
	void SetInputSystem(TestInputSystem* Input);

	HWND hwnd;

	void Tick(double DeltaTime);

	void AddListener(AxisBinding AxisBind);
	void AddListener(KeyBinding KeyBind);

	void ProcessRaw(int deltaX, int deltaY);

	std::vector<AxisBinding> AxisRawBindings;
	std::vector<AxisBinding> AxisBindings;
	std::vector<KeyBinding> KeyBindings;

	float step = 0.05f;

	float lastDeltaX = 0.0f;
	float lastDeltaY = 0.0f;
	float deltaX = 0.0f;
	float deltaY = 0.0f;

	POINT currentMousePos;
	POINT lastMousePos;

	int currentMousePosX;
	int currentMousePosY;

	int lastMousePosX;
	int lastMousePosY;

	TestInputSystem* Input;

	static WindowKeyMessageHandler* Get();

	FKeyEvent Event;

private:
	WindowKeyMessageHandler();
};

DECLARE_LOG_CATEGORY_EXTERN(MessageHandlerLog)