#include "WindowKeyMessageHandler.h"
#include <string>
#include <algorithm>

DEFINE_LOG_CATEGORY(MessageHandlerLog)

WindowKeyMessageHandler::WindowKeyMessageHandler()
{
	// Engine key binding
	EngineKeyBindings.emplace_back("EngineExit", EKeys::F4, false, /* Alt */ true);

	AxisBindings.emplace_back("MoveCameraForward", 1.0f, EKeys::W);
	AxisBindings.emplace_back("MoveCameraForward", -1.0f, EKeys::S);
	AxisBindings.emplace_back("MoveCameraRight", -1.0f, EKeys::A);
	AxisBindings.emplace_back("MoveCameraRight", 1.0f, EKeys::D);
	AxisBindings.emplace_back("MoveCameraUp", 1.0f, EKeys::E);
	AxisBindings.emplace_back("MoveCameraUp", -1.0f, EKeys::F);

	AxisRawBindings.emplace_back("Turn", 1.0f, EKeys::MouseX);
	AxisRawBindings.emplace_back("LookUp", 1.0f, EKeys::MouseY);

	KeyBindings.emplace_back("TestAction", EKeys::SpaceBar);
	KeyBindings.emplace_back("ToggleCameraInput", EKeys::F2, /* Shift */ true);
}

WindowKeyMessageHandler::~WindowKeyMessageHandler()
{
	EngineInput = nullptr;
}

void WindowKeyMessageHandler::SetHwnd(HWND hwnd)
{
	this->hwnd = hwnd;
}

void WindowKeyMessageHandler::SetEngineInputSystem(InputSystem* Input)
{
	EngineInput = Input;
}

void WindowKeyMessageHandler::AddInputSystem(InputSystem* Input)
{
	Inputs.emplace(Input);
}

void WindowKeyMessageHandler::RemoveInputSystem(InputSystem* Input)
{
	Inputs.erase(Input);
}

void WindowKeyMessageHandler::ProcessInputSystem(InputSystem* Input, double DeltaTime)
{
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
					if (GetAsyncKeyState(*keycode))
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
				GetCursorPos(&LastMousePos);
				if (ScreenToClient(hwnd, &LastMousePos))
				{
					if (axisRawItem.TestKey == EKeys::MouseX)
					{
						if (DeltaX != LastDeltaX)
						{
							if (DeltaX > LastDeltaX)
							{
								axisRawItem.Scale = 1.0f;
								axisRawItem.KeyEvent = EInputEvent::IE_Axis;
							}
							else
							{
								axisRawItem.Scale = -1.0f;
								axisRawItem.KeyEvent = EInputEvent::IE_Axis;
							}

							auto deltaRawX = int(std::abs(DeltaX - LastDeltaX));
							for (int i = 0; i < deltaRawX; i++)
							{
								Input->ExecuteAxis(axisRawItem.AxisName, axisRawItem.Scale);
							}

							LastDeltaX = DeltaX;
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
						if (DeltaY != LastDeltaY)
						{
							if (DeltaY > LastDeltaY)
							{
								axisRawItem.Scale = 1.0f;
								axisRawItem.KeyEvent = EInputEvent::IE_Axis;
							}
							else
							{
								axisRawItem.Scale = -1.0f;
								axisRawItem.KeyEvent = EInputEvent::IE_Axis;
							}

							auto deltaRawX = int(std::abs(DeltaY - LastDeltaY));
							for (int i = 0; i < deltaRawX; i++)
							{
								Input->ExecuteAxis(axisRawItem.AxisName, axisRawItem.Scale);
							}

							LastDeltaY = DeltaY;
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

void WindowKeyMessageHandler::ProcessEngineInputSystem(InputSystem* Input, double DeltaTime)
{
	if (Input != nullptr)
	{
		for (auto& Item : EngineKeyBindings)
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

		if (!EngineAxisBindings.empty())
		{
			for (auto& axisItem : EngineAxisBindings)
			{
				const unsigned int* keycode = nullptr;
				const unsigned int* charcode = nullptr;
				FInputKeyManager::Get().GetCodesFromKey(axisItem.TestKey, keycode, charcode);

				if (keycode != nullptr)
				{
					if (GetAsyncKeyState(*keycode))
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

		if (!EngineAxisRawBindings.empty())
		{
			for (auto& axisRawItem : EngineAxisRawBindings)
			{
				GetCursorPos(&LastMousePos);
				if (ScreenToClient(hwnd, &LastMousePos))
				{
					if (axisRawItem.TestKey == EKeys::MouseX)
					{
						if (DeltaX != LastDeltaX)
						{
							if (DeltaX > LastDeltaX)
							{
								axisRawItem.Scale = 1.0f;
								axisRawItem.KeyEvent = EInputEvent::IE_Axis;
							}
							else
							{
								axisRawItem.Scale = -1.0f;
								axisRawItem.KeyEvent = EInputEvent::IE_Axis;
							}

							auto deltaRawX = int(std::abs(DeltaX - LastDeltaX));
							for (int i = 0; i < deltaRawX; i++)
							{
								Input->ExecuteAxis(axisRawItem.AxisName, axisRawItem.Scale);
							}

							LastDeltaX = DeltaX;
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
						if (DeltaY != LastDeltaY)
						{
							if (DeltaY > LastDeltaY)
							{
								axisRawItem.Scale = 1.0f;
								axisRawItem.KeyEvent = EInputEvent::IE_Axis;
							}
							else
							{
								axisRawItem.Scale = -1.0f;
								axisRawItem.KeyEvent = EInputEvent::IE_Axis;
							}

							auto deltaRawX = int(std::abs(DeltaY - LastDeltaY));
							for (int i = 0; i < deltaRawX; i++)
							{
								Input->ExecuteAxis(axisRawItem.AxisName, axisRawItem.Scale);
							}

							LastDeltaY = DeltaY;
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

void WindowKeyMessageHandler::Tick(double DeltaTime)
{
	Event.ProcessModifierKeys();

	ProcessEngineInputSystem(EngineInput, DeltaTime);

	if(!Inputs.empty())
	{
		for (auto const& Input : Inputs)
		{
			if(Input != nullptr)
			{
				ProcessInputSystem(Input, DeltaTime);
			}
		}
	}
}

void WindowKeyMessageHandler::ProcessRaw(int deltaX, int deltaY)
{
	this->DeltaX += (float)deltaX;
	this->DeltaY += (float)deltaY;
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
