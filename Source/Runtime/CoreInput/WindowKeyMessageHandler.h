#pragma once
#include <stack>
#include <Runtime/Core/Core.h>
#include <Runtime/CoreInput/InputSystem.h>
#include <set>
#include <vector>
#include <map>
#include <memory>
#include <Runtime/CoreInput/InputTypes.h>

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
	HWND hwnd = nullptr;

	void Tick(double DeltaTime);
	void ProcessRaw(int deltaX, int deltaY);

	void AddInputSystem(InputSystem* Input);
	void RemoveInputSystem(InputSystem* Input);
	void ProcessInputSystem(InputSystem* inInput, double DeltaTime);
	// Object Inputs
	std::set<InputSystem*> Inputs;
	std::vector<AxisBinding> AxisRawBindings;
	std::vector<AxisBinding> AxisBindings;
	std::vector<KeyBinding> KeyBindings;

	float LastDeltaX = 0.0f;
	float LastDeltaY = 0.0f;
	float DeltaX = 0.0f;
	float DeltaY = 0.0f;

	POINT LastMousePos;

	void ProcessEngineInputSystem(InputSystem* Input, double DeltaTime);
	void SetEngineInputSystem(InputSystem* Input);
	// Engine input
	InputSystem* EngineInput;
	std::vector<AxisBinding> EngineAxisRawBindings;
	std::vector<AxisBinding> EngineAxisBindings;
	std::vector<KeyBinding> EngineKeyBindings;

	static WindowKeyMessageHandler* Get();

	FKeyEvent Event;

private:
	WindowKeyMessageHandler();
};

DECLARE_LOG_CATEGORY_EXTERN(MessageHandlerLog)