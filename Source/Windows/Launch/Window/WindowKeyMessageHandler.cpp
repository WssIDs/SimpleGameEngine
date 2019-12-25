#include "WindowKeyMessageHandler.h"
#include "Graphics\DX11\Math\WGMath.h"
#include <map>
#include <string>
#include <algorithm>

DEFINE_LOG_CATEGORY(MessageHandlerLog)

WindowKeyMessageHandler::WindowKeyMessageHandler()
{
	AxisBindings.push_back(AxisBinding("MoveCameraForward", 1.0f, EKeys::W));
	AxisBindings.push_back(AxisBinding("MoveCameraForward", -1.0f, EKeys::S));
	AxisBindings.push_back(AxisBinding("MoveCameraRight", -1.0f, EKeys::A));
	AxisBindings.push_back(AxisBinding("MoveCameraRight", 1.0f, EKeys::D));
	AxisBindings.push_back(AxisBinding("MoveCameraUp", 1.0f, EKeys::E));
	AxisBindings.push_back(AxisBinding("MoveCameraUp", -1.0f, EKeys::F));

	AxisRawBindings.push_back(AxisBinding("Turn", 1.0f, EKeys::MouseX));
	AxisRawBindings.push_back(AxisBinding("LookUp", 1.0f, EKeys::MouseY));

	KeyBindings.push_back(KeyBinding("TestAction", EKeys::SpaceBar));
	KeyBindings.push_back(KeyBinding("ToggleCameraInput", EKeys::F2, /* Shift */ true));
}

WindowKeyMessageHandler::~WindowKeyMessageHandler()
{
	Input = nullptr;
}

void WindowKeyMessageHandler::SetHwnd(HWND hwnd)
{
	this->hwnd = hwnd;
}

void WindowKeyMessageHandler::SetInputSystem(InputSystem* Input)
{
	this->Input = Input;
}

void WindowKeyMessageHandler::Tick(double DeltaTime)
{
	Event.ProcessModifierKeys();

	if (Input != nullptr)
	{
		for (auto& Item : KeyBindings)
		{
			const unsigned int* keycode = nullptr;
			const unsigned int* charcode = nullptr;
			FInputKeyManager::Get().GetCodesFromKey(Item.TestKey, keycode, charcode);

			if (keycode != nullptr)
			{
				if (Event.IsShiftDown() == Item.IsShiftDown() && Event.IsAltDown() == Item.IsAltDown() && Event.IsControlDown() == Item.IsControlDown() && Event.IsCommandDown() == Item.IsCommandDown())
				{
					bool KeyIsPressed = (GetAsyncKeyState(*keycode) & 0x8000) != 0;

					if (!Item.KeyIsDown() && KeyIsPressed && Item.KeyIsUp())
					{
						Item.KeyEvent = EInputEvent::IE_Pressed;
						WGE_LOG(MessageHandlerLog, LogVerbosity::Warning, "Key Press = %s", Item.TestKey.GetDisplayName().c_str());
						Input->ExecuteKey(Item.ActionName, Item.KeyEvent);
					}
				}

				bool KeyIsReleased = (GetAsyncKeyState(*keycode) & 0x8000) == 0;

				if (!Item.KeyIsUp() && KeyIsReleased && Item.KeyIsDown())
				{
					Item.KeyEvent = EInputEvent::IE_Released;
					WGE_LOG(MessageHandlerLog, LogVerbosity::Warning, "Key Release = %s", Item.TestKey.GetDisplayName().c_str());
					Input->ExecuteKey(Item.ActionName, Item.KeyEvent);
				}
			}
			if (charcode != nullptr)
			{
				if (Event.IsShiftDown() == Item.IsShiftDown() && Event.IsAltDown() == Item.IsAltDown() && Event.IsControlDown() == Item.IsControlDown() && Event.IsCommandDown() == Item.IsCommandDown())
				{
					bool KeyIsPressed = (GetAsyncKeyState(VkKeyScan(*charcode)) & 0x8000) != 0;

					if (!Item.KeyIsDown() && KeyIsPressed)
					{
						Item.KeyEvent = EInputEvent::IE_Pressed;
						Input->ExecuteKey(Item.ActionName, Item.KeyEvent);
					}
				}

				bool KeyIsReleased = (GetAsyncKeyState(VkKeyScan(*charcode)) & 0x8000) == 0;

				if (!Item.KeyIsUp() && KeyIsReleased)
				{
					Item.KeyEvent = EInputEvent::IE_Released;
					Input->ExecuteKey(Item.ActionName, Item.KeyEvent);
				}
			}
		}

		if (!AxisBindings.empty())
		{
			for (auto& axisItem : AxisBindings)
			{
				const unsigned int* keycode = nullptr;
				const unsigned int* charcode = nullptr;
				FInputKeyManager::Get().GetCodesFromKey(axisItem.TestKey, keycode, charcode);

				if (keycode != nullptr)
				{
					if(GetAsyncKeyState(*keycode))
					{
						axisItem.KeyEvent = EInputEvent::IE_Axis;
						axisItem.Scale = WGMath::FInterpConstantTo(axisItem.Scale, axisItem.MaxScale, (float)DeltaTime, 7.0f);
						//WGE_LOG(MessageHandlerLog, LogVerbosity::Warning, "Axis Key Release = %s", axisItem.TestKey.GetDisplayName().c_str());
						Input->ExecuteAxis(axisItem.AxisName, axisItem.Scale);
					}
					else
					{
						axisItem.KeyEvent = EInputEvent::IE_MAX;
						axisItem.Scale = WGMath::FInterpConstantTo(axisItem.Scale, 0, (float)DeltaTime, 7.0f);
						Input->ExecuteAxis(axisItem.AxisName, axisItem.Scale);
					}
				}
				if (charcode != nullptr)
				{
					if (GetAsyncKeyState(*charcode))
					{
						axisItem.KeyEvent = EInputEvent::IE_Axis;
						axisItem.Scale = WGMath::FInterpConstantTo(axisItem.Scale, axisItem.MaxScale, (float)DeltaTime, 7.0f);
						//WGE_LOG(MessageHandlerLog, LogVerbosity::Warning, "Axis Char = %s", axisItem.TestKey.GetDisplayName().c_str());
						Input->ExecuteAxis(axisItem.AxisName, axisItem.Scale);
					}
					else
					{
						axisItem.KeyEvent = EInputEvent::IE_MAX;
						axisItem.Scale = WGMath::FInterpConstantTo(axisItem.Scale, 0, (float)DeltaTime, 7.0f);
						Input->ExecuteAxis(axisItem.AxisName, axisItem.Scale);
					}
				}
			}
		}

		/////////////////   WITH RAW ///////////////////

		if (!AxisRawBindings.empty())
		{
			for (auto& axisRawItem : AxisRawBindings)
			{
				GetCursorPos(&lastMousePos);
				if (ScreenToClient(hwnd, &lastMousePos))
				{
					if (axisRawItem.TestKey == EKeys::MouseX)
					{
						if (deltaX != lastDeltaX)
						{
							if (deltaX > lastDeltaX)
							{
								axisRawItem.Scale = 1.0f;
								axisRawItem.KeyEvent = EInputEvent::IE_Axis;
							}
							else
							{
								axisRawItem.Scale = -1.0f;
								axisRawItem.KeyEvent = EInputEvent::IE_Axis;
							}

							auto deltaRawX = int(std::abs(deltaX - lastDeltaX));
							for (int i = 0; i < deltaRawX; i++)
							{
								Input->ExecuteAxis(axisRawItem.AxisName, axisRawItem.Scale);
							}

							lastDeltaX = deltaX;
						}
						else
						{
							axisRawItem.KeyEvent = EInputEvent::IE_MAX;
							axisRawItem.Scale = WGMath::FInterpConstantTo(axisRawItem.Scale, 0.0f, (float)DeltaTime, 7.0f);
							Input->ExecuteAxis(axisRawItem.AxisName, 0);
						}
					}

					if (axisRawItem.TestKey == EKeys::MouseY)
					{
						if (deltaY != lastDeltaY)
						{
							if (deltaY > lastDeltaY)
							{
								axisRawItem.Scale = 1.0f;
								axisRawItem.KeyEvent = EInputEvent::IE_Axis;
							}
							else
							{
								axisRawItem.Scale = -1.0f;
								axisRawItem.KeyEvent = EInputEvent::IE_Axis;
							}

							auto deltaRawX = int(std::abs(deltaY - lastDeltaY));
							for (int i = 0; i < deltaRawX; i++)
							{
								Input->ExecuteAxis(axisRawItem.AxisName, axisRawItem.Scale);
							}

							lastDeltaY = deltaY;
						}
						else
						{
							axisRawItem.KeyEvent = EInputEvent::IE_MAX;
							axisRawItem.Scale = WGMath::FInterpConstantTo(axisRawItem.Scale, 0.0f, (float)DeltaTime, 7.0f);
							Input->ExecuteAxis(axisRawItem.AxisName, 0);
						}
					}
				}
			}
		}
	}
}

void WindowKeyMessageHandler::AddListener(AxisBinding AxisBind)
{
	AxisBindings.push_back(AxisBind);
}

void WindowKeyMessageHandler::AddListener(KeyBinding KeyBind)
{
	KeyBindings.push_back(KeyBind);
}

void WindowKeyMessageHandler::ProcessRaw(int deltaX, int deltaY)
{
	this->deltaX += (float)deltaX;
	this->deltaY += (float)deltaY;
}

WindowKeyMessageHandler* WindowKeyMessageHandler::Get()
{
	static WindowKeyMessageHandler* instance = new WindowKeyMessageHandler();
	return instance;
}

bool FKeyEvent::ProcessModifierKeyDown(FKey inKey)
{
	const unsigned int* keycode = nullptr;
	const unsigned int* charcode = nullptr;
	FInputKeyManager::Get().GetCodesFromKey(inKey, keycode, charcode);

	bool KeyIsPressed = (GetAsyncKeyState(*keycode) & 0x8000) != 0;

	if (inKey.IsModifierKey() && KeyIsPressed)
	{
		//WGE_LOG(MessageHandlerLog, LogVerbosity::Warning, "Key = %s, KeyCode = %d", inKey.GetDisplayName().c_str(), KeyIsPressed);
		return true;
	}

	return false;
}
